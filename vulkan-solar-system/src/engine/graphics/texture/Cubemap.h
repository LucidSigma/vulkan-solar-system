#pragma once

#include "../../utility/interfaces/INoncopyable.h"
#include "../../utility/interfaces/INonmovable.h"

#include <cstdint>
#include <string>

#include <glm/glm.hpp>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

class Cubemap
	: private INoncopyable, private INonmovable
{
private:
	static constexpr std::uint32_t s_CubeSides = 6u;

	glm::uvec2 m_size{ 0, 0 };
	std::uint32_t m_mipLevels = 1;

	const class Renderer& m_renderer;

	VkImage m_image = VK_NULL_HANDLE;
	VkImageView m_imageView = VK_NULL_HANDLE;
	VmaAllocation m_allocation = VK_NULL_HANDLE;

public:
	static constexpr std::uint32_t SideCount() noexcept { return s_CubeSides; }

	Cubemap(const class Renderer& renderer, const std::string& folderPath, const std::string& imageExtension, const bool generateMipmaps = true);
	~Cubemap() noexcept;

	inline VkImageView GetImageView() const noexcept { return m_imageView; }
	inline const glm::uvec2& GetSize() const noexcept { return m_size; }
	inline std::uint32_t GetMipLevels() const noexcept { return m_mipLevels; }

private:
	void Initialise(const std::string& folderPath, const std::string& imageExtension, const bool generateMipmaps);
	void Destroy() noexcept;

	void GenerateMipmaps();
};