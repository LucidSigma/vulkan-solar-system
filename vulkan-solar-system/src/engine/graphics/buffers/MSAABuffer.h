#pragma once

#include "../../utility/interfaces/INoncopyable.h"
#include "../../utility/interfaces/INonmovable.h"

#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

class MSAABuffer
	: private INoncopyable, private INonmovable
{
private:
	const class VulkanContext& m_vulkanContext;

	VkImage m_image = VK_NULL_HANDLE;
	VkImageView m_imageView = VK_NULL_HANDLE;

	VmaAllocation m_allocation = VK_NULL_HANDLE;

public:
	MSAABuffer(const class VulkanContext& vulkanContext);
	~MSAABuffer() noexcept;

	void Initialise(const VkExtent2D& swapchainExtent, const VkFormat format);
	void Destroy() noexcept;

	inline VkImageView GetImageView() const noexcept { return m_imageView; }
};