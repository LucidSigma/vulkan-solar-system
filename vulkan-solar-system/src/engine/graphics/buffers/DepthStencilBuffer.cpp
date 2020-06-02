#include "DepthStencilBuffer.h"

#include <cstdint>
#include <vector>

#include "../renderer/VulkanContext.h"
#include "../renderer/VulkanUtility.h"

DepthStencilBuffer::DepthStencilBuffer(const VulkanContext& vulkanContext)
	: m_vulkanContext(vulkanContext)
{
	FindFormat();
}

DepthStencilBuffer::~DepthStencilBuffer() noexcept
{
	Destroy();
}

void DepthStencilBuffer::Initialise(const VkExtent2D& swapchainExtent, const bool enableMultisample)
{
	vulkan_util::ImageInfo imageInfo{ };
	imageInfo.size = glm::tvec2<std::uint32_t>{ swapchainExtent.width, swapchainExtent.height };
	imageInfo.mipLevels = 1;
	imageInfo.sampleCount = enableMultisample ? m_vulkanContext.GetMSAASampleCount() : VK_SAMPLE_COUNT_1_BIT;
	imageInfo.format = m_format;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	imageInfo.memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY;
	imageInfo.isCubemap = false;

	vulkan_util::CreateImage(m_vulkanContext, imageInfo, m_image, m_allocation);
	m_imageView = vulkan_util::CreateImageView(m_vulkanContext, m_image, m_format, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

	vulkan_util::TransitionInfo transitionInfo{ };
	transitionInfo.arrayLayerCount = 1;
	transitionInfo.mipLevels = 1;
	transitionInfo.format = m_format;
	transitionInfo.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	transitionInfo.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	transitionInfo.supportsStencil = m_supportsStencil;

	vulkan_util::TransitionImageLayout(m_vulkanContext, m_image, transitionInfo);
}

void DepthStencilBuffer::Destroy() noexcept
{
	if (m_image != VK_NULL_HANDLE)
	{
		vkDestroyImageView(m_vulkanContext.GetLogicalDevice(), m_imageView, nullptr);
		m_imageView = VK_NULL_HANDLE;

		vmaDestroyImage(m_vulkanContext.GetAllocator(), m_image, m_allocation);
		m_image = VK_NULL_HANDLE;
		m_allocation = VK_NULL_HANDLE;
	}
}

void DepthStencilBuffer::FindFormat()
{
	static const std::vector<VkFormat> candidateDepthStencilFormats{
		VK_FORMAT_D32_SFLOAT_S8_UINT,
		VK_FORMAT_D24_UNORM_S8_UINT,
		VK_FORMAT_D32_SFLOAT,
		VK_FORMAT_D16_UNORM_S8_UINT,
		VK_FORMAT_D16_UNORM
	};

	m_format = vulkan_util::FindSupportedFormat(m_vulkanContext, candidateDepthStencilFormats, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);

	if (m_format != VK_FORMAT_D32_SFLOAT && m_format != VK_FORMAT_D16_UNORM)
	{
		m_supportsStencil = true;
	}
}