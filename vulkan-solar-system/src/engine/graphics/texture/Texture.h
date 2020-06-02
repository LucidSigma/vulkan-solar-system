#pragma once

#include "../../utility/interfaces/INoncopyable.h"
#include "../../utility/interfaces/INonmovable.h"

#include <cstdint>
#include <string_view>

#include <glm/glm.hpp>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

class Texture
	: private INoncopyable, private INonmovable
{
private:
	glm::uvec2 m_size{ 0, 0 };
	std::uint32_t m_mipLevels = 1;

	const class Renderer& m_renderer;

	VkImage m_image = VK_NULL_HANDLE;
	VkImageView m_imageView = VK_NULL_HANDLE;
	VmaAllocation m_allocation = VK_NULL_HANDLE;

public:
	Texture(const class Renderer& renderer, const std::string_view& filepath, const bool generateMipmaps = true);
	~Texture() noexcept;

	inline VkImageView GetImageView() const noexcept { return m_imageView; }
	inline const glm::uvec2& GetSize() const noexcept { return m_size; }
	inline std::uint32_t GetMipLevels() const noexcept { return m_mipLevels; }

private:
	void Initialise(const std::string_view& filepath, const bool generateMipmaps);
	void Destroy() noexcept;

	void GenerateMipmaps();
};