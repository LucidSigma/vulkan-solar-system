#pragma once

#include "../../utility/interfaces/INoncopyable.h"
#include "../../utility/interfaces/INonmovable.h"

#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

class DepthStencilBuffer
	: private INoncopyable, private INonmovable
{
private:
	const class VulkanContext& m_vulkanContext;

	VkImage m_image = VK_NULL_HANDLE;
	VkImageView m_imageView = VK_NULL_HANDLE;

	VmaAllocation m_allocation = VK_NULL_HANDLE;

	VkFormat m_format = VK_FORMAT_UNDEFINED;
	bool m_supportsStencil = false;

public:
	DepthStencilBuffer(const class VulkanContext& vulkanContext);
	~DepthStencilBuffer() noexcept;

	void Initialise(const VkExtent2D& swapchainExtent, const bool enableMultisample);
	void Destroy() noexcept;

	inline VkImageView GetImageView() const noexcept { return m_imageView; }
	inline VkFormat GetFormat() const noexcept { return m_format; }
	inline bool SupportsStencilOperations() const noexcept { return m_supportsStencil; }

private:
	void FindFormat();
};