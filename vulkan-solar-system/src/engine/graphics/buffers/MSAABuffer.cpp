#include "MSAABuffer.h"

#include <cstdint>
#include <vector>

#include "../renderer/VulkanContext.h"
#include "../renderer/VulkanUtility.h"

MSAABuffer::MSAABuffer(const VulkanContext& vulkanContext)
	: m_vulkanContext(vulkanContext)
{ }

MSAABuffer::~MSAABuffer() noexcept
{
	Destroy();
}

void MSAABuffer::Initialise(const VkExtent2D& swapchainExtent, const VkFormat format)
{
	vulkan_util::ImageInfo imageInfo{ };
	imageInfo.size = glm::tvec2<std::uint32_t>{ swapchainExtent.width, swapchainExtent.height };
	imageInfo.mipLevels = 1;
	imageInfo.sampleCount = m_vulkanContext.GetMSAASampleCount();
	imageInfo.format = format;
	imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageInfo.usage = static_cast<VkImageUsageFlagBits>(VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT);
	imageInfo.memoryUsage = VMA_MEMORY_USAGE_GPU_ONLY;
	imageInfo.isCubemap = false;

	vulkan_util::CreateImage(m_vulkanContext, imageInfo, m_image, m_allocation);
	m_imageView = vulkan_util::CreateImageView(m_vulkanContext, m_image, format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}

void MSAABuffer::Destroy() noexcept
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