#pragma once

#include "../utility/interfaces/INoncopyable.h"
#include "../utility/interfaces/INonmovable.h"

#include <cstdint>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <tiny_obj_loader/tiny_obj_loader.h>

#include "../graphics/buffers/IndexBuffer.h"
#include "../graphics/buffers/VertexBuffer.h"
#include "../graphics/Vertex.h"

class Model
	: private INoncopyable, private INonmovable
{
private:
	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;

public:
	Model(const class Renderer& renderer, const std::string_view& meshFilepath, const bool includeNormals = true);
	~Model() noexcept = default;

	void Draw(class Renderer& renderer, const std::uint32_t instanceCount = 1) const;

private:
	void LoadOBJFile(const std::string_view& filepath, const bool includeNormals);

	template <typename V>
	void LoadVertices(const std::vector<tinyobj::shape_t>& shapes, const tinyobj::attrib_t& attribute, std::vector<std::uint32_t>& indices)
	{
		static_assert(false, "Invalid vertex type.");
	}

	template <>
	void LoadVertices<VertexP3T2>(const std::vector<tinyobj::shape_t>& shapes, const tinyobj::attrib_t& attribute, std::vector<std::uint32_t>& indices)
	{
		std::vector<VertexP3T2> vertices;
		std::unordered_map<VertexP3T2, std::uint32_t> uniqueVertices;

		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				VertexP3T2 vertex{ };

				vertex.position = glm::vec3{
					attribute.vertices[3 * index.vertex_index + 0],
					attribute.vertices[3 * index.vertex_index + 1],
					attribute.vertices[3 * index.vertex_index + 2]
				};

				vertex.textureCoordinates = glm::vec2{
					attribute.texcoords[2 * index.texcoord_index + 0],
					1.0f - attribute.texcoords[2 * index.texcoord_index + 1]
				};

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<std::uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}
		}

		m_vertexBuffer.Initialise(vertices);
	}

	template <>
	void LoadVertices<VertexP3T2N3>(const std::vector<tinyobj::shape_t>& shapes, const tinyobj::attrib_t& attribute, std::vector<std::uint32_t>& indices)
	{
		std::vector<VertexP3T2N3> vertices;
		std::unordered_map<VertexP3T2N3, std::uint32_t> uniqueVertices;

		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				VertexP3T2N3 vertex{ };

				vertex.position = glm::vec3{
					attribute.vertices[3 * index.vertex_index + 0],
					attribute.vertices[3 * index.vertex_index + 1],
					attribute.vertices[3 * index.vertex_index + 2]
				};

				vertex.textureCoordinates = glm::vec2{
					attribute.texcoords[2 * index.texcoord_index + 0],
					1.0f - attribute.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.normal = glm::vec3{
					attribute.normals[3 * index.normal_index + 0],
					attribute.normals[3 * index.normal_index + 1],
					attribute.normals[3 * index.normal_index + 2]
				};

				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<std::uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}
		}

		m_vertexBuffer.Initialise(vertices);
	}
};