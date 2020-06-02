#pragma once

#include <cstdint>

#include <glm/glm.hpp>
#include <vma/vk_mem_alloc.h>
#include <vulkan/vulkan.h>

#include "VulkanContext.h"

namespace vulkan_util
{
	struct BufferInfo
	{
		VkDeviceSize size;
		VkBufferUsageFlags usageFlags;
		VmaMemoryUsage memoryUsage;
	};

	struct ImageInfo
	{
		glm::tvec2<std::uint32_t> size;

		std::uint32_t mipLevels;
		VkSampleCountFlagBits sampleCount;

		VkFormat format;
		VkImageTiling tiling;
		VkImageUsageFlagBits usage;
		VmaMemoryUsage memoryUsage;

		bool isCubemap;
	};

	struct TransitionInfo
	{
		std::uint32_t arrayLayerCount;
		std::uint32_t mipLevels;

		VkFormat format;
		VkImageLayout oldLayout;
		VkImageLayout newLayout;

		bool supportsStencil;
	};

	extern void CreateBuffer(const VulkanContext& vulkanContext, const BufferInfo& bufferInfo, VkBuffer& buffer, VmaAllocation& bufferAllocation);
	extern void CopyBuffer(const VulkanContext& vulkanContext, const VkBuffer sourceBuffer, const VkBuffer destinationBuffer, const VkDeviceSize size);
	extern void CopyBufferToImage(const VulkanContext& vulkanContext, const VkBuffer sourceBuffer, const VkImage destinationImage, const std::uint32_t imageWidth, const std::uint32_t imageHeight, const std::uint32_t arrayLayer = 0);

	extern void CreateImage(const VulkanContext& vulkanContext, const ImageInfo& imageInfo, VkImage& image, VmaAllocation& imageAlloaction);
	[[nodiscard]] extern VkImageView CreateImageView(const VulkanContext& vulkanContext, const VkImage image, const VkFormat format, const VkImageAspectFlags imageAspect, const std::uint32_t mipLevels);
	[[nodiscard]] extern VkImageView CreateCubeImageView(const VulkanContext& vulkanContext, const VkImage image, const VkFormat format, const VkImageAspectFlags imageAspect, const std::uint32_t mipLevels);
	[[nodiscard]] extern VkFormat FindSupportedFormat(const VulkanContext& vulkanContext, const std::vector<VkFormat>& candidateFormats, const VkImageTiling imageTiling, const VkFormatFeatureFlags features);

	[[nodiscard]] extern VkCommandBuffer BeginSingleTimeCommands(const VulkanContext& vulkanContext);
	extern void EndSingleTimeCommands(const VulkanContext& vulkanContext, VkCommandBuffer commandBuffer);
	extern void TransitionImageLayout(const VulkanContext& vulkanContext, const VkImage image, const TransitionInfo& transitionInfo);
}