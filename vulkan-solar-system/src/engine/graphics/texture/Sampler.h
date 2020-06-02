#pragma once

#include "../../utility/interfaces/INoncopyable.h"
#include "../../utility/interfaces/INonmovable.h"

#include <cstdint>
#include <optional>
#include <type_traits>

#include <vulkan/vulkan.h>

class Sampler
	: private INoncopyable, private INonmovable
{
public:
	enum class Filter
		: std::underlying_type_t<VkFilter>
	{
		Nearest = VK_FILTER_NEAREST,
		Linear = VK_FILTER_LINEAR
	};

	enum class AddressMode
		: std::underlying_type_t<VkSamplerAddressMode>
	{
		Repeat = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		MirroredRepeat = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
		ClampToEdge = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		MirroredClampToEdge = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
		ClampToBorder = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
	};

	struct Config
	{
		Filter minFilter = Filter::Linear;
		Filter maxFilter = Filter::Linear;

		AddressMode uAddressMode = AddressMode::Repeat;
		AddressMode vAddressMode = AddressMode::Repeat;
		AddressMode wAddressMode = AddressMode::Repeat;

		std::optional<float> maxAnisotropy = 16.0f;
		std::uint32_t mipLevels = 1;
	};

private:
	const class Renderer& m_renderer;

	VkSampler m_handle = VK_NULL_HANDLE;

public:
	Sampler(const class Renderer& renderer, const Config& config);
	~Sampler() noexcept;

	void Destroy() noexcept;

	inline VkSampler GetHandle() const noexcept { return m_handle; }

private:
	void Initialise(const Config& config);
};