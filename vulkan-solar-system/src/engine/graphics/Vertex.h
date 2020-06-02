#pragma once

#include <cstddef>
#include <functional>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

struct VertexP3
{
	glm::vec3 position;

	inline bool operator ==(const VertexP3& other) const noexcept = default;
};

struct VertexP2C3
{
	glm::vec2 position;
	glm::vec3 colour;

	inline bool operator ==(const VertexP2C3& other) const noexcept = default;
};

struct VertexP3C3
{
	glm::vec3 position;
	glm::vec3 colour;

	inline bool operator ==(const VertexP3C3& other) const noexcept = default;
};

struct VertexP3T2
{
	glm::vec3 position;
	glm::vec2 textureCoordinates;

	inline bool operator ==(const VertexP3T2& other) const noexcept = default;
};

struct VertexP3C3T2
{
	glm::vec3 position;
	glm::vec3 colour;
	glm::vec2 textureCoordinates;

	inline bool operator ==(const VertexP3C3T2& other) const noexcept = default;
};

struct VertexP3T2N3
{
	glm::vec3 position;
	glm::vec2 textureCoordinates;
	glm::vec3 normal;

	inline bool operator ==(const VertexP3T2N3& other) const noexcept = default;
};

namespace std
{
	template <>
	struct hash<VertexP3>
	{
		inline std::size_t operator ()(const VertexP3& vertex) const noexcept
		{
			return hash<glm::vec3>()(vertex.position);
		}
	};

	template <>
	struct hash<VertexP2C3>
	{
		inline std::size_t operator ()(const VertexP2C3& vertex) const noexcept
		{
			return hash<glm::vec2>()(vertex.position) ^ (hash<glm::vec3>()(vertex.colour) << 1);
		}
	};

	template <>
	struct hash<VertexP3C3>
	{
		inline std::size_t operator ()(const VertexP3C3& vertex) const noexcept
		{
			return hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.colour) << 1);
		}
	};

	template <>
	struct hash<VertexP3T2>
	{
		inline std::size_t operator ()(const VertexP3T2& vertex) const noexcept
		{
			return hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec2>()(vertex.textureCoordinates) << 1);
		}
	};

	template <>
	struct hash<VertexP3C3T2>
	{
		inline std::size_t operator ()(const VertexP3C3T2& vertex) const noexcept
		{
			return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.colour) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.textureCoordinates) << 1);
		}
	};

	template <>
	struct hash<VertexP3T2N3>
	{
		inline std::size_t operator ()(const VertexP3T2N3& vertex) const noexcept
		{
			return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec2>()(vertex.textureCoordinates) << 1)) >> 1) ^ (hash<glm::vec3>()(vertex.normal) << 1);
		}
	};
}