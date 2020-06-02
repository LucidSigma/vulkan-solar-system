#include "SolarSystem.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../engine/Application.h"

SolarSystem::SolarSystem(Application& application)
	: Scene(application)
{ }

void SolarSystem::OnLoad()
{
	m_projection = glm::perspectiveLH(glm::radians(m_camera.GetZoom()), static_cast<float>(m_application.GetWindow().GetDrawableSize().x) / static_cast<float>(m_application.GetWindow().GetDrawableSize().y), s_NearPlane, s_FarPlane);
	m_projection[1][1] *= -1.0f;

	{
		std::ifstream configFile("data/solar_system.json");

		if (!configFile.is_open())
		{
			throw std::runtime_error("Failed to open planet data file.");
		}

		std::ostringstream configStringStream;
		configStringStream << configFile.rdbuf();
		configFile.close();

		m_solarSystemData = nlohmann::json::parse(configStringStream.str());
	}

	m_samplers.Add("main", m_application.GetRenderer(), Sampler::Config{
		.minFilter = Sampler::Filter::Linear,
		.maxFilter = Sampler::Filter::Linear,

		.uAddressMode = Sampler::AddressMode::Repeat,
		.vAddressMode = Sampler::AddressMode::Repeat,
		.wAddressMode = Sampler::AddressMode::Repeat,

		.maxAnisotropy = m_application.GetConfig()["enable-anisotropy"] ? std::optional(m_application.GetRenderer().GetVulkanContext().GetMaxAnisotropy()) : std::nullopt,
		.mipLevels = m_application.GetConfig()["enable-mipmaps"] ? 13u : 1u
	});

	m_sphereModel = std::make_unique<Model>(m_application.GetRenderer(), "assets/models/solar_system/planet.obj");

	m_skybox = std::make_unique<Skybox>(m_application.GetRenderer(), "assets/textures/cubemaps/milky_way", "png");
	m_pipelines.Add("skybox", m_application.GetRenderer(), GraphicsPipeline::Config{
		.shaderInfo{
			{ "assets/shaders/skybox.vert.spv", ShaderModule::Stage::Vertex },
			{ "assets/shaders/skybox.frag.spv", ShaderModule::Stage::Fragment }
		},
		.areShadersPreCompiled = true,

		.enableDepthTest = false,
		.drawWireframe = false,

		.blending = GraphicsPipeline::Config::Blending::Alpha,
		.cullFace = GraphicsPipeline::Config::CullFace::Clockwise
	});

	m_pipelines["skybox"].SetTexture(1, m_skybox->GetCubemap(), m_samplers["main"]);

	LoadCelestialObjects();
}

void SolarSystem::OnUnload() noexcept
{ }

void SolarSystem::PollEvents(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			m_application.FinishCurrentScene();

		default:
			break;
		}

		break;

	default:
		break;
	}
}

void SolarSystem::ProcessInput(const input::KeyboardState& keyboardState, const input::MouseState& mouseState, const std::int32_t scrollState)
{
	m_camera.ProcessInput(keyboardState, mouseState, scrollState);

	if (keyboardState.GetKeyDown(SDL_SCANCODE_TAB))
	{
		m_camera.SetPosition(glm::vec3{ 0.0f, 0.0f, 0.0f });
	}

	if (mouseState.GetButtonDown(input::MouseState::MouseButton::Left))
	{
		m_timeScale = 1.0f;
	}
	else if (mouseState.GetButtonDown(input::MouseState::MouseButton::Middle))
	{
		m_timeScale = 0.1f;
	}
	else if (mouseState.GetButtonDown(input::MouseState::MouseButton::Right))
	{
		m_timeScale = 0.01f;
	}
	else if (keyboardState.GetKeyDown(SDL_SCANCODE_RSHIFT))
	{
		m_timeScale = 0.001f;
	}
	else if (keyboardState.GetKeyDown(SDL_SCANCODE_RETURN))
	{
		m_timeScale = 0.0f;
	}
}

void SolarSystem::Update(const float deltaTime)
{
	const float scaledDeltaTime = deltaTime * m_timeScale;
	
	if (m_camera.HasZoomChanged())
	{
		m_projection = glm::perspectiveLH(glm::radians(m_camera.GetZoom()), static_cast<float>(m_application.GetWindow().GetDrawableSize().x) / static_cast<float>(m_application.GetWindow().GetDrawableSize().y), s_NearPlane, s_FarPlane);
		m_projection[1][1] *= -1.0f;
	}

	m_camera.Update(deltaTime);
	m_sol->Update(scaledDeltaTime);

	for (const auto& [planetName, planet] : m_planets.GetAssets())
	{
		planet->Update(scaledDeltaTime);
	}

	const std::array<glm::mat4, 2u> vp{ m_camera.GetViewMatrix(), m_projection };

	for (const auto& [pipelineName, pipeline] : m_pipelines.GetAssets())
	{
		if (pipelineName == "bloom") [[unlikely]]
		{
			continue;
		}

		if (pipelineName == "skybox")
		{
			const std::array<glm::mat4, 2u> mp{ m_skybox->GetModelViewMatrix(m_camera), m_projection };
			pipeline->SetUniform(0, mp);
		}
		else [[likely]]
		{
			pipeline->SetUniform(0, vp);
			pipeline->SetUniform(2, m_camera.GetPosition());
		}
	}
}

void SolarSystem::Render(Renderer& renderer) const
{
	m_skybox->Draw(renderer, m_pipelines["skybox"]);
	m_sol->Draw(renderer, m_pipelines);

	for (const auto& [planetName, planet] : m_planets.GetAssets())
	{
		const float angle = glm::degrees(glm::abs(glm::acos(glm::dot(
			glm::normalize(m_camera.GetFront()),
			glm::normalize(planet->GetPosition() - m_camera.GetPosition()
		)))));

		if (angle < m_camera.GetZoom() * 1.5f || glm::distance(planet->GetPosition(), m_camera.GetPosition()) < 100.0f)
		{
			planet->Draw(renderer, m_pipelines);
		}
	}

	// Draw rings after all planets due to transparency/depth buffer.
	for (const auto ringedPlanet : m_ringedPlanets)
	{
		ringedPlanet->DrawRings(renderer, m_pipelines);
	}
}

void SolarSystem::ProcessWindowResize()
{
	for (const auto& [pipelineName, pipeline] : m_pipelines.GetAssets())
	{
		pipeline->RefreshUniformBuffers();
	}

	m_projection = glm::perspectiveLH(glm::radians(m_camera.GetZoom()), static_cast<float>(m_application.GetWindow().GetDrawableSize().x) / static_cast<float>(m_application.GetWindow().GetDrawableSize().y), s_NearPlane, s_FarPlane);
	m_projection[1][1] *= -1.0f;
}

void SolarSystem::LoadCelestialObjects()
{
	const Sampler& mainSampler = m_samplers["main"];
	m_sol = std::make_unique<Sol>(m_application.GetRenderer(), "sol", m_solarSystemData["sol"]);

	m_pipelines.Add("sol", m_application.GetRenderer(), GraphicsPipeline::Config{
		.shaderInfo{
			{ "assets/shaders/no_phong.vert.spv", ShaderModule::Stage::Vertex },
			{ "assets/shaders/no_phong.frag.spv", ShaderModule::Stage::Fragment }
		},
		.areShadersPreCompiled = true,

		.enableDepthTest = true,
		.drawWireframe = false,

		.blending = GraphicsPipeline::Config::Blending::Alpha,
		.cullFace = GraphicsPipeline::Config::CullFace::Clockwise
	});

	m_pipelines["sol"].SetTexture(1, m_sol->GetTexture(), mainSampler);

	for (const auto& [planetName, planetData] : m_solarSystemData["planets"].items())
	{
		m_planets.Add(planetName, m_application.GetRenderer(), *m_sphereModel, planetName, planetData);

		m_pipelines.Add(planetName, m_application.GetRenderer(), GraphicsPipeline::Config{
			.shaderInfo{
				{ "assets/shaders/model.vert.spv", ShaderModule::Stage::Vertex },
				{ "assets/shaders/model.frag.spv", ShaderModule::Stage::Fragment }
			},
			.areShadersPreCompiled = true,

			.enableDepthTest = true,
			.drawWireframe = false,

			.blending = GraphicsPipeline::Config::Blending::Alpha,
			.cullFace = GraphicsPipeline::Config::CullFace::Clockwise
		});

		m_pipelines[planetName].SetTexture(1, m_planets[planetName].GetTexture(), mainSampler);

		if (planetData.contains("rings"))
		{
			const std::string& ringName = planetName + "-rings";

			m_pipelines.Add(ringName, m_application.GetRenderer(), GraphicsPipeline::Config{
				.shaderInfo{
					{ "assets/shaders/model.vert.spv", ShaderModule::Stage::Vertex },
					{ "assets/shaders/model.frag.spv", ShaderModule::Stage::Fragment }
				},
				.areShadersPreCompiled = true,

				.enableDepthTest = true,
				.drawWireframe = false,

				.blending = GraphicsPipeline::Config::Blending::Alpha,
				.cullFace = GraphicsPipeline::Config::CullFace::Clockwise
			});

			m_pipelines[ringName].SetTexture(1, m_planets[planetName].GetRingsTexture(), mainSampler);
		}

		if (planetData.contains("moons"))
		{
			for (const auto& [moonName, moonData] : planetData["moons"].items())
			{
				m_pipelines.Add(moonName, m_application.GetRenderer(), GraphicsPipeline::Config{
					.shaderInfo{
						{ "assets/shaders/model.vert.spv", ShaderModule::Stage::Vertex },
						{ "assets/shaders/model.frag.spv", ShaderModule::Stage::Fragment }
					},
					.areShadersPreCompiled = true,

					.enableDepthTest = true,
					.drawWireframe = false,

					.blending = GraphicsPipeline::Config::Blending::Alpha,
					.cullFace = GraphicsPipeline::Config::CullFace::Clockwise
				});

				m_pipelines[moonName].SetTexture(1, m_planets[planetName].GetMoonTexture(moonName), mainSampler);
			}
		}
	}

	for (const auto& [planetName, planet] : m_planets.GetAssets())
	{
		if (planet->HasRings())
		{
			m_ringedPlanets.push_back(planet.get());
		}
	}
}