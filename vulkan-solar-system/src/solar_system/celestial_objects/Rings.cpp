#include "Rings.h"

#include <cstdint>
#include <vector>

#include "../../engine/graphics/renderer/Renderer.h"
#include "../../engine/graphics/Vertex.h"

Rings::Rings(const Renderer& renderer, const std::string& planetName, const nlohmann::json& data)
	: m_vertexBuffer(renderer), m_indexBuffer(renderer), m_name(planetName + "-rings"),
	  m_rotationVelocity(static_cast<float>(data["rotation"]["velocity"])), m_inclination(static_cast<float>(data["rotation"]["inclination"])),
	  m_scale(static_cast<float>(data["scale"]))
{
	InitialiseBuffers();
	InitialiseTexture(renderer, planetName, data["image-extension"].get<std::string>());
}

void Rings::Update(const float deltaTime)
{
	m_rotationAngle += m_rotationVelocity * deltaTime;

	m_model = glm::rotate(GetModelMatrix(), -m_rotationAngle, glm::vec3{ 0.0f, 1.0f, 0.0f });
	m_model = glm::rotate(GetModelMatrix(), glm::radians(m_inclination), glm::vec3{ 1.0, 0.0, 0.0f });
	m_model = glm::scale(GetModelMatrix(), glm::vec3{ m_scale, 1.0f, m_scale });
}

void Rings::Draw(Renderer& renderer) const
{
	renderer.BindVertexBuffer(m_vertexBuffer);
	renderer.BindIndexBuffer(m_indexBuffer);

	renderer.DrawIndexed(m_indexBuffer.GetIndexCount());
}

void Rings::InitialiseBuffers()
{
	static const std::vector<VertexP3T2N3> ringsVertices{
		// Top face.
		{ { -1.0f, 0.0f, -1.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },
		{ { -1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
		{ { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
		{ { 1.0f, 0.0f, -1.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } },

		// Bottom face.
		{ { -1.0f, 0.0f, -1.0f }, { 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f } },
		{ { -1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },
		{ { 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f } },
		{ { 1.0f, 0.0f, -1.0f }, { 1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f } }
	};

	static const std::vector<std::uint32_t> ringsIndices{
		// Top face.
		0, 1, 2,
		2, 3, 0,

		// Bottom face.
		5, 4, 6,
		7, 6, 4
	};

	m_vertexBuffer.Initialise(ringsVertices);
	m_indexBuffer.Initialise(ringsIndices);
}

void Rings::InitialiseTexture(const Renderer& renderer, const std::string& planetName, const std::string& imageExtension)
{
	const std::string texturePath = "assets/models/solar_system/" + planetName + "/rings." + imageExtension;
	m_texture = std::make_unique<Texture>(renderer, texturePath);
}