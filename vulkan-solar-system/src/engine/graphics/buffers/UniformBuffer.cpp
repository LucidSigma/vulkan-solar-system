#include "UniformBuffer.h"

#include <cstring>

#include "../renderer/Renderer.h"
#include "../renderer/VulkanUtility.h"

UniformBuffer::UniformBuffer(const Renderer& renderer)
	: m_renderer(renderer)
{ }

UniformBuffer::~UniformBuffer() noexcept
{
	Destroy();
}

void UniformBuffer::Initialise(const VkDeviceSize size)
{
	m_bufferSize = size;

	Create();
}

void UniformBuffer::SetBufferData(const void* bufferData, const std::size_t size, const VkDeviceSize offset)
{
	void* uniformData = nullptr;

	vmaMapMemory(m_renderer.GetVulkanContext().GetAllocator(), m_allocation, &uniformData);
	{
		std::memcpy(reinterpret_cast<void*>(reinterpret_cast<std::byte*>(uniformData) + offset), bufferData, size);
	}
	vmaUnmapMemory(m_renderer.GetVulkanContext().GetAllocator(), m_allocation);
}

void UniformBuffer::Destroy() noexcept
{
	if (m_bufferHandle != VK_NULL_HANDLE)
	{
		vmaDestroyBuffer(m_renderer.GetVulkanContext().GetAllocator(), m_bufferHandle, m_allocation);
		m_bufferHandle = VK_NULL_HANDLE;
		m_allocation = VK_NULL_HANDLE;
	}
}

void UniformBuffer::Create()
{
	if (m_bufferSize == 0)
	{
		return;
	}

	vulkan_util::BufferInfo bufferInfo{ };
	bufferInfo.size = m_bufferSize;
	bufferInfo.usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	bufferInfo.memoryUsage = VMA_MEMORY_USAGE_CPU_ONLY;

	vulkan_util::CreateBuffer(m_renderer.GetVulkanContext(), bufferInfo, m_bufferHandle, m_allocation);
}