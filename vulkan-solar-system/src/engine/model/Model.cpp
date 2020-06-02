#include "Model.h"

#include <filesystem>
#include <string>

#include "../graphics/renderer/Renderer.h"

Model::Model(const class Renderer& renderer, const std::string_view& meshFilepath, const bool includeNormals)
	: m_vertexBuffer(renderer), m_indexBuffer(renderer)
{
	LoadOBJFile(meshFilepath, includeNormals);
}

void Model::Draw(Renderer& renderer, const std::uint32_t instanceCount) const
{
	renderer.BindVertexBuffer(m_vertexBuffer);
	renderer.BindIndexBuffer(m_indexBuffer);

	if (instanceCount == 1)
	{
		renderer.DrawIndexed(m_indexBuffer.GetIndexCount());
	}
	else
	{
		renderer.DrawInstancedIndex(m_indexBuffer.GetIndexCount(), instanceCount);
	}
}

void Model::LoadOBJFile(const std::string_view& filepath, const bool includeNormals)
{
	tinyobj::attrib_t attribute{ };

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warning;
	std::string error;

	if (!tinyobj::LoadObj(&attribute, &shapes, &materials, &warning, &error, filepath.data()))
	{
		throw std::runtime_error(warning + "\n" + error);
	}

	std::vector<std::uint32_t> indices;

	if (includeNormals)
	{
		LoadVertices<VertexP3T2N3>(shapes, attribute, indices);
	}
	else
	{
		LoadVertices<VertexP3T2>(shapes, attribute, indices);
	}

	m_indexBuffer.Initialise(indices);
}