#include "Planet.h"

#include "../../engine/graphics/pipeline/GraphicsPipeline.h"
#include "../../engine/graphics/renderer/Renderer.h"

Planet::Planet(const Renderer& renderer, const Model& model, const std::string& name, const nlohmann::json& data)
	: m_model(model), m_name(name), m_ringsName(name + "-rings"),
	  m_distanceFromSun(static_cast<float>(data["orbit"]["distance-from-sun"])), m_orbitVelocity(static_cast<float>(data["orbit"]["velocity"])), m_orbitInclination(static_cast<float>(data["orbit"]["inclination"])),
	  m_rotationVelocity(static_cast<float>(data["rotation"]["velocity"])), m_axialTilt(static_cast<float>(data["rotation"]["axial-tilt"])),
	  m_scale(glm::vec3{ static_cast<float>(data["scale"]["x"]) , static_cast<float>(data["scale"]["y"]) , static_cast<float>(data["scale"]["z"]) })
{
	if (data.contains("rings"))
	{
		CreateRings(renderer, data["rings"]);
	}

	if (data.contains("moons"))
	{
		PopulateMoons(renderer, data["moons"]);
	}

	const std::string texturePath = "assets/models/solar_system/" + name + "/" + name + "." + data["image-extension"].get<std::string>();
	m_texture = std::make_unique<Texture>(renderer, texturePath);
}

void Planet::Update(const float deltaTime)
{
	m_orbitAngle += m_orbitVelocity * deltaTime;
	m_rotationAngle += m_rotationVelocity * deltaTime;

	m_modelMatrix = glm::rotate(glm::mat4{ 1.0f }, -m_orbitAngle, glm::vec3{ 0.0f, 1.0f, 0.0f });
	m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3{ m_distanceFromSun, glm::sin(m_orbitAngle) * m_orbitInclination, m_distanceFromSun });
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_axialTilt), glm::vec3{ 0.0, 0.0, 1.0f });

	m_position = glm::vec3(m_modelMatrix * glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f });
	
	for (auto& [moonName, moon] : m_moons)
	{
		moon->SetModelMatrix(m_modelMatrix);
	}

	if (m_rings != nullptr)
	{
		m_rings->SetModelMatrix(m_modelMatrix);
	}

	m_modelMatrix = glm::rotate(GetModelMatrix(), -m_rotationAngle, glm::vec3{ 0.0f, 1.0f, 0.0f });
	m_modelMatrix = glm::scale(GetModelMatrix(), m_scale);

	if (m_rings != nullptr)
	{
		m_rings->Update(deltaTime);
	}

	for (const auto& [moonName, moon] : m_moons)
	{
		moon->Update(deltaTime);
	}
}

void Planet::Draw(Renderer& renderer, const AssetManager<GraphicsPipeline>& pipelines) const
{
	const GraphicsPipeline& planetPipeline = pipelines[m_name];

	renderer.BindPipeline(planetPipeline);
	renderer.BindDescriptorSet(planetPipeline);
	renderer.PushConstants(planetPipeline, GetModelMatrix());
	m_model.Draw(renderer);

	for (const auto& [moonName, moon] : m_moons)
	{
		const GraphicsPipeline& moonPipeline = pipelines[moonName];

		renderer.BindPipeline(moonPipeline);
		renderer.BindDescriptorSet(moonPipeline);
		renderer.PushConstants(moonPipeline, moon->GetModelMatrix());
		moon->Draw(renderer);
	}
}

void Planet::DrawRings(Renderer& renderer, const AssetManager<GraphicsPipeline>& pipelines) const
{
	const GraphicsPipeline& ringsPipeline = pipelines[m_ringsName];

	renderer.BindPipeline(ringsPipeline);
	renderer.BindDescriptorSet(ringsPipeline);
	renderer.PushConstants(ringsPipeline, m_rings->GetModelMatrix());

	m_rings->Draw(renderer);
}

void Planet::CreateRings(const Renderer& renderer, const nlohmann::json& ringData)
{
	m_rings = std::make_unique<Rings>(renderer, m_name, ringData);
}

void Planet::PopulateMoons(const Renderer& renderer, const nlohmann::json& moonData)
{
	m_moons.reserve(moonData.size());

	for (const auto& [moonName, data] : moonData.items())
	{
		m_moons.insert({ moonName, std::make_unique<Moon>(renderer, m_model, m_name, moonName, data) });
	}
}