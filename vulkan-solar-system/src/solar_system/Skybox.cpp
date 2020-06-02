#include "Skybox.h"

#include <vector>

#include "../engine/graphics/renderer/Renderer.h"
#include "../engine/graphics/pipeline/GraphicsPipeline.h"
#include "../engine/graphics/Vertex.h"

Skybox::Skybox(const Renderer& renderer, const std::string& folderPath, const std::string& imageExtension)
	: m_vertices(renderer), m_indices(renderer)
{
	InitialiseBuffers();
	InitialiseCubemap(renderer, folderPath, imageExtension);
}

void Skybox::Draw(Renderer& renderer, const GraphicsPipeline& pipeline) const
{
	renderer.BindPipeline(pipeline);
	renderer.BindDescriptorSet(pipeline);

	renderer.BindVertexBuffer(m_vertices);
	renderer.BindIndexBuffer(m_indices);
	
	renderer.DrawIndexed(m_indices.GetIndexCount());
}

void Skybox::InitialiseBuffers()
{
	static const std::vector<VertexP3> vertices{
		// Back face.
		{ { -1.0f, -1.0f, -1.0f } },
		{ { 1.0f, -1.0f, -1.0f } },
		{ { 1.0f, 1.0f, -1.0f } },
		{ { -1.0f, 1.0f, -1.0f } },

		// Right face.
		{ { 1.0f, -1.0f, -1.0f } },
		{ { 1.0f, -1.0f, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f } },
		{ { 1.0f, 1.0f, -1.0f } },

		// Front face.
		{ { 1.0f, -1.0f, 1.0f } },
		{ { -1.0f, -1.0f, 1.0f } },
		{ { -1.0f, 1.0f, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f } },

		// Left face.
		{ { -1.0f, -1.0f, 1.0f } },
		{ { -1.0f, -1.0f, -1.0f } },
		{ { -1.0f, 1.0f, -1.0f } },
		{ { -1.0f, 1.0f, 1.0f } },

		// Bottom face.
		{ { 1.0f, -1.0f, -1.0f } },
		{ { -1.0f, -1.0f, -1.0f } },
		{ { -1.0f, -1.0f, 1.0f } },
		{ { 1.0f, -1.0f, 1.0f } },

		// Top face.
		{ { 1.0f, 1.0f, 1.0f } },
		{ { -1.0f, 1.0f, 1.0f } },
		{ { -1.0f, 1.0f, -1.0f } },
		{ { 1.0f, 1.0f, -1.0f } }
	};

	static const std::vector<std::uint16_t> indices{
		// Back face.
		0, 1, 2,
		2, 3, 0,

		// Right face.
		4, 5, 6,
		6, 7, 4,

		// Front face.
		8, 9, 10,
		10, 11, 8,

		// Left face.
		12, 13, 14,
		14, 15, 12,

		// Bottom face.
		16, 17, 18,
		18, 19, 16,

		// Top face.
		20, 21, 22,
		22, 23, 20
	};

	m_vertices.Initialise(vertices);
	m_indices.Initialise(indices);
}

void Skybox::InitialiseCubemap(const Renderer& renderer, const std::string& folderPath, const std::string& imageExtension)
{
	m_cubemap = std::make_unique<Cubemap>(renderer, folderPath, imageExtension);
}