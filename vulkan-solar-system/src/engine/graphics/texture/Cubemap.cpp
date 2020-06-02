#include "Cubemap.h"

#include <array>
#include <cmath>
#include <stdexcept>

#include <stb_image/stb_image.h>

#include "../renderer/VulkanUtility.h"
#include "../renderer/Renderer.h"

Cubemap::Cubemap(const Renderer& renderer, const std::string& folderPath, const std::string& imageExtension, const bool generateMipmaps)
	: m_renderer(renderer)
{
	Initialise(folderPath, imageExtension, generateMipmaps);
}

Cubemap::~Cubemap() noexcept
{
	Destroy();
}

void Cubemap::Initialise(const std::string& folderPath, const std::string& imageExtension, const bool generateMipmaps)
{
	constexpr std::array<const char*, s_CubeSides> SideFileNames{
		"px", "nx",
		"py", "ny",
		"pz", "nz"
	};

	std::uint32_t currentArrayLayer = 0;

	for (const auto sideFileName : SideFileNames)
	{
		const std::string currentSideFilePath = folderPath + "/" + sideFileName + "." + imageExtension;

		int textureWidth = 0;
		int textureHeight = 0;

		stbi_uc* pixels = stbi_load(currentSideFilePath.c_str(), &textureWidth, &textureHeight, nullptr, STBI_rgb_alpha);

		if (pixels == nullptr)
		{
			throw std::invalid_argument("Failed to load texture image: " + currentSideFilePath);
		}

		if (m_image == VK_NULL_HANDLE)
		{
			m_size = glm::uvec2{ textureWidth, textureHeight };
			m_mipLevels = generateMipmaps ? static_cast<std::uint32_t>(std::floor(std::log2(std::max(m_size.x, m_size.y)))) + 1 : 1;

			vulkan_util::ImageInfo imageInfo{ };
			imageInfo.size = glm::tvec2<std::uint32_t>{ textureWidth, textureHeight };
			imageInfo.mipLevels = m_mipLevels;
			imageInfo.sampleCount = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
			imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			imageInfo.usage = static_cast<VkImageUsageFlagBits>(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
			imageInfo.memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY;
			imageInfo.isCubemap = true;

			vulkan_util::CreateImage(m_renderer.GetVulkanContext(), imageInfo, m_image, m_allocation);

			vulkan_util::TransitionInfo transitionInfo{ };
			transitionInfo.arrayLayerCount = s_CubeSides;
			transitionInfo.mipLevels = m_mipLevels;
			transitionInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
			transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			transitionInfo.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			transitionInfo.supportsStencil = false;

			vulkan_util::TransitionImageLayout(m_renderer.GetVulkanContext(), m_image, transitionInfo);
		}

		VkDeviceSize textureImageSize = static_cast<VkDeviceSize>(textureWidth) * static_cast<VkDeviceSize>(textureHeight) * 4;
		VkBuffer stagingBuffer = VK_NULL_HANDLE;
		VmaAllocation stagingAllocation = VK_NULL_HANDLE;

		vulkan_util::BufferInfo stagingBufferInfo{ };
		stagingBufferInfo.size = textureImageSize;
		stagingBufferInfo.usageFlags = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stagingBufferInfo.memoryUsage = VMA_MEMORY_USAGE_CPU_ONLY;

		vulkan_util::CreateBuffer(m_renderer.GetVulkanContext(), stagingBufferInfo, stagingBuffer, stagingAllocation);

		void* textureData = nullptr;

		vmaMapMemory(m_renderer.GetVulkanContext().GetAllocator(), stagingAllocation, &textureData);
		{
			std::memcpy(textureData, pixels, static_cast<std::size_t>(textureImageSize));
		}
		vmaUnmapMemory(m_renderer.GetVulkanContext().GetAllocator(), stagingAllocation);

		stbi_image_free(pixels);
		pixels = nullptr;

		vulkan_util::CopyBufferToImage(m_renderer.GetVulkanContext(), stagingBuffer, m_image, static_cast<std::uint32_t>(textureWidth), static_cast<std::uint32_t>(textureHeight), currentArrayLayer++);

		vmaDestroyBuffer(m_renderer.GetVulkanContext().GetAllocator(), stagingBuffer, stagingAllocation);
		stagingBuffer = VK_NULL_HANDLE;
		stagingAllocation = VK_NULL_HANDLE;
	}

	// Mipmaps currently do not work properly.
	//if (generateMipmaps)
	//{
	//	GenerateMipmaps();
	//}
	//else
	{
		vulkan_util::TransitionInfo transitionInfo{ };
		transitionInfo.arrayLayerCount = s_CubeSides;
		transitionInfo.mipLevels = m_mipLevels;
		transitionInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		transitionInfo.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		transitionInfo.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		transitionInfo.supportsStencil = false;

		vulkan_util::TransitionImageLayout(m_renderer.GetVulkanContext(), m_image, transitionInfo);
	}

	m_imageView = vulkan_util::CreateCubeImageView(m_renderer.GetVulkanContext(), m_image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels);
}

void Cubemap::Destroy() noexcept
{
	if (m_image != VK_NULL_HANDLE)
	{
		vkDestroyImageView(m_renderer.GetVulkanContext().GetLogicalDevice(), m_imageView, nullptr);
		m_imageView = VK_NULL_HANDLE;

		vmaDestroyImage(m_renderer.GetVulkanContext().GetAllocator(), m_image, m_allocation);
		m_image = VK_NULL_HANDLE;
		m_allocation = VK_NULL_HANDLE;
	}
}

void Cubemap::GenerateMipmaps()
{
	VkFormatProperties formatProperties{ };
	vkGetPhysicalDeviceFormatProperties(m_renderer.GetVulkanContext().GetPhysicalDevice(), VK_FORMAT_R8G8B8A8_SRGB, &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
	{
		throw std::runtime_error("Vulkan physical device texture image format does not support linear blitting - mipmaps could not be generated, but were requested.");
	}

	for (std::uint32_t arrayLayer = 0; arrayLayer < s_CubeSides; ++arrayLayer)
	{
		VkCommandBuffer commandBuffer = vulkan_util::BeginSingleTimeCommands(m_renderer.GetVulkanContext());

		VkImageMemoryBarrier imageMemoryBarrier{ };
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.image = m_image;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
		imageMemoryBarrier.subresourceRange.layerCount = s_CubeSides;
		imageMemoryBarrier.subresourceRange.levelCount = 1;

		std::int32_t mipWidth = m_size.x;
		std::int32_t mipHeight = m_size.y;

		for (std::uint32_t i = 1; i < m_mipLevels; ++i)
		{
			imageMemoryBarrier.subresourceRange.baseMipLevel = i - 1;
			imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

			VkImageBlit imageBlit{ };
			imageBlit.srcOffsets[0] = VkOffset3D{ 0, 0, 0 };
			imageBlit.srcOffsets[1] = VkOffset3D{ mipWidth, mipHeight, 1 };
			imageBlit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlit.srcSubresource.mipLevel = i - 1;
			imageBlit.srcSubresource.baseArrayLayer = arrayLayer;
			imageBlit.srcSubresource.layerCount = 1;
			imageBlit.dstOffsets[0] = VkOffset3D{ 0, 0, 0 };
			imageBlit.dstOffsets[1] = VkOffset3D{ mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
			imageBlit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageBlit.dstSubresource.mipLevel = i;
			imageBlit.dstSubresource.baseArrayLayer = arrayLayer;
			imageBlit.dstSubresource.layerCount = 1;

			vkCmdBlitImage(commandBuffer, m_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &imageBlit, VK_FILTER_LINEAR);

			imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

			if (mipWidth > 1)
			{
				mipWidth /= 2;
			}

			if (mipHeight > 1)
			{
				mipHeight /= 2;
			}
		}

		imageMemoryBarrier.subresourceRange.baseMipLevel = m_mipLevels - 1;
		imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

		vulkan_util::EndSingleTimeCommands(m_renderer.GetVulkanContext(), commandBuffer);
		commandBuffer = VK_NULL_HANDLE;
	}
}