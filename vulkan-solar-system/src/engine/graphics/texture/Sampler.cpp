#include "Sampler.h"

#include "../renderer/Renderer.h"

Sampler::Sampler(const Renderer& renderer, const Config& config)
	: m_renderer(renderer)
{
	Initialise(config);
}

Sampler::~Sampler() noexcept
{
	Destroy();
}

void Sampler::Initialise(const Config& config)
{
	VkSamplerCreateInfo samplerCreateInfo{ };
	samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerCreateInfo.minFilter = static_cast<VkFilter>(config.minFilter);
	samplerCreateInfo.magFilter = static_cast<VkFilter>(config.maxFilter);
	samplerCreateInfo.addressModeU = static_cast<VkSamplerAddressMode>(config.uAddressMode);
	samplerCreateInfo.addressModeV = static_cast<VkSamplerAddressMode>(config.vAddressMode);
	samplerCreateInfo.addressModeW = static_cast<VkSamplerAddressMode>(config.wAddressMode);

	if (config.maxAnisotropy.has_value())
	{
		samplerCreateInfo.anisotropyEnable = VK_TRUE;
		samplerCreateInfo.maxAnisotropy = config.maxAnisotropy.value();
	}
	else
	{
		samplerCreateInfo.anisotropyEnable = VK_FALSE;
		samplerCreateInfo.maxAnisotropy = 1.0f;
	}

	samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
	samplerCreateInfo.compareEnable = VK_FALSE;
	samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
	samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerCreateInfo.mipLodBias = 0.0f;
	samplerCreateInfo.minLod = 0.0f;
	samplerCreateInfo.maxLod = static_cast<float>(config.mipLevels);

	if (vkCreateSampler(m_renderer.GetVulkanContext().GetLogicalDevice(), &samplerCreateInfo, nullptr, &m_handle) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Vulkan texture sampler.");
	}
}

void Sampler::Destroy() noexcept
{
	if (m_handle != VK_NULL_HANDLE)
	{
		vkDestroySampler(m_renderer.GetVulkanContext().GetLogicalDevice(), m_handle, nullptr);
		m_handle = VK_NULL_HANDLE;
	}
}