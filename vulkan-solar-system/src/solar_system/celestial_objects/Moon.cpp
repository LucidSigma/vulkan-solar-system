#include "Moon.h"

#include "../../engine/graphics/renderer/Renderer.h"

Moon::Moon(const Renderer& renderer, const Model& model, const std::string& planetName, const std::string& name, const nlohmann::json& data)
	: m_model(model), m_distanceFromPlanet(static_cast<float>(data["orbit"]["distance-from-planet"])), m_orbitVelocity(static_cast<float>(data["orbit"]["velocity"])), m_orbitInclination(static_cast<float>(data["orbit"]["inclination"])),
	  m_rotationVelocity(static_cast<float>(data["rotation"]["velocity"])), m_axialTilt(static_cast<float>(data["rotation"]["axial-tilt"])),
	  m_scale(glm::vec3{ static_cast<float>(data["scale"]["x"]), static_cast<float>(data["scale"]["y"]), static_cast<float>(data["scale"]["z"]) })
{
	const std::string texturePath = "assets/models/solar_system/" + planetName + "/" + name + "/" + name + "." + data["image-extension"].get<std::string>();
	m_texture = std::make_unique<Texture>(renderer, texturePath);
}

void Moon::Update(const float deltaTime)
{
	m_orbitAngle += m_orbitVelocity * deltaTime;
	m_rotationAngle += m_rotationVelocity * deltaTime;

	m_modelMatrix = glm::rotate(GetModelMatrix(), -m_orbitAngle, glm::vec3{ 0.0f, 1.0f, 0.0f });
	m_modelMatrix = glm::translate(GetModelMatrix(), glm::vec3{ m_distanceFromPlanet, glm::sin(m_orbitAngle) * -m_orbitInclination, m_distanceFromPlanet });
	m_modelMatrix = glm::rotate(GetModelMatrix(), -m_rotationAngle, glm::vec3{ 0.0f, 1.0f, 0.0f });
	m_modelMatrix = glm::rotate(GetModelMatrix(), glm::radians(m_axialTilt), glm::vec3{ 1.0, 0.0, 0.0f });
	m_modelMatrix = glm::scale(GetModelMatrix(), m_scale);
}

void Moon::Draw(Renderer& renderer) const
{
	m_model.Draw(renderer);
}