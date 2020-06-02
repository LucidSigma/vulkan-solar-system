#pragma once

#include "../engine/scene/Scene.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <json/json.hpp>

#include "../engine/camera/Camera3D.h"
#include "../engine/graphics/pipeline/GraphicsPipeline.h"
#include "../engine/graphics/texture/Sampler.h"
#include "../engine/model/Model.h"
#include "../engine/utility/AssetManager.h"

#include "celestial_objects/Planet.h"
#include "celestial_objects/Sol.h"
#include "Skybox.h"

class SolarSystem final
	: public Scene
{
private:
	static constexpr float s_NearPlane = 0.1f;
	static constexpr float s_FarPlane = 10'000.0f;
	
	AssetManager<GraphicsPipeline> m_pipelines;
	AssetManager<Sampler> m_samplers;

	std::unique_ptr<Model> m_sphereModel = nullptr;
	std::unique_ptr<Skybox> m_skybox = nullptr;

	std::unique_ptr<Sol> m_sol = nullptr;
	AssetManager<Planet> m_planets;
	std::vector<const Planet*> m_ringedPlanets;
	nlohmann::json m_solarSystemData;

	Camera3D m_camera{ glm::vec3{ 0.0f, 100.0f, 0.0f } };

	glm::mat4 m_projection{ 1.0f };
	float m_timeScale = 1.0f;

public:
	explicit SolarSystem(class Application& application);
	virtual ~SolarSystem() noexcept override = default;

	virtual void OnLoad() override;
	virtual void OnUnload() noexcept override;

	virtual void PollEvents(const SDL_Event& event) override;
	virtual void ProcessInput(const input::KeyboardState& keyboardState, const input::MouseState& mouseState, const std::int32_t scrollState) override;
	virtual void Update(const float deltaTime) override;
	virtual void Render(class Renderer& renderer) const override;

	virtual void ProcessWindowResize() override;

private:
	void LoadCelestialObjects();
};