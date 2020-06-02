#include "Sol.h"

#include "../../engine/graphics/renderer/Renderer.h"

Sol::Sol(const Renderer& renderer, const std::string& name, const nlohmann::json& data)
	: m_name(name), m_rotationVelocity(static_cast<float>(data["rotation-velocity"])),
	  m_scale(glm::vec3{ static_cast<float>(data["scale"]["x"]), static_cast<float>(data["scale"]["y"]), static_cast<float>(data["scale"]["z"]) })
{
	m_model = std::make_unique<Model>(renderer, "assets/models/solar_system/planet.obj", false);

	const std::string texturePath = "assets/models/solar_system/" + name + "/" + name + "." + data["image-extension"].get<std::string>();
	m_texture = std::make_unique<Texture>(renderer, texturePath);
}

void Sol::Update(const float deltaTime)
{
	m_rotationAngle += m_rotationVelocity * deltaTime;

	m_modelMatrix = glm::rotate(glm::mat4{ 1.0f }, -m_rotationAngle, glm::vec3{ 0.0f, 1.0f, 0.0f });
	m_modelMatrix = glm::scale(m_modelMatrix, m_scale);
}

void Sol::Draw(Renderer& renderer, const AssetManager<class GraphicsPipeline>& pipelines) const
{
	const GraphicsPipeline& pipeline = pipelines[m_name];

	renderer.BindPipeline(pipeline);
	renderer.BindDescriptorSet(pipeline);
	renderer.PushConstants(pipeline, m_modelMatrix);
	m_model->Draw(renderer);
}

void Sol::DrawGlow(Renderer& renderer, const GraphicsPipeline& pipeline) const
{
	renderer.BindPipeline(pipeline);
	renderer.BindDescriptorSet(pipeline);
	renderer.PushConstants(pipeline, m_modelMatrix);
	m_model->Draw(renderer);
}