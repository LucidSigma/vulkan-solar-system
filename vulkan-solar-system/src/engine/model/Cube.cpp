#include "../graphics/renderer/Renderer.h"
#include "Cube.h"

Cube::Cube(const Renderer& renderer)
	: m_vertexBuffer(renderer), m_indexBuffer(renderer)
{
	m_vertexBuffer.Initialise(s_vertices);
	m_indexBuffer.Initialise(s_indices);
}

void Cube::Draw(Renderer& renderer, const std::uint32_t instanceCount) const
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