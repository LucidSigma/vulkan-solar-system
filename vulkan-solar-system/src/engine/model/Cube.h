#pragma once

#include "../utility/interfaces/INoncopyable.h"
#include "../utility/interfaces/INonmovable.h"

#include <cstdint>
#include <vector>

#include "../graphics/buffers/IndexBuffer.h"
#include "../graphics/buffers/VertexBuffer.h"
#include "../graphics/Vertex.h"

class Cube
	: private INoncopyable, private INonmovable
{
private:
	inline static const std::vector<VertexP3C3T2> s_vertices{
		// Back face.
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f } },
		{ { 1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
		{ { 1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
		{ { -1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },

		// Right face.
		{ { 1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
		{ { 1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
		{ { 1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },

		// Front face.
		{ { 1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
		{ { -1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },

		// Left face.
		{ { -1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { -1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },

		// Bottom face.
		{ { 1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
		{ { -1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { -1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { 1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },

		// Top face.
		{ { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
		{ { -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
		{ { -1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f } },
		{ { 1.0f, 1.0f, -1.0f }, { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } }
	};

	inline static const std::vector<std::uint16_t> s_indices{
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

	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;

	glm::mat4 m_modelMatrix{ 1.0f };

public:
	explicit Cube(const class Renderer& renderer);
	~Cube() noexcept = default;

	void Draw(class Renderer& renderer, const std::uint32_t instanceCount = 1) const;

	inline const glm::mat4& GetModelMatrix() const noexcept { return m_modelMatrix; }
	inline void SetModelMatrix(const glm::mat4 & modelMatrix) noexcept { m_modelMatrix = modelMatrix; }
};