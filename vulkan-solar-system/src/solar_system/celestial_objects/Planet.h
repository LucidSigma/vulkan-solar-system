#pragma once
#include "../../engine/utility/AssetManager.h"
#include "../../engine/utility/interfaces/INoncopyable.h"
#include "../../engine/utility/interfaces/INonmovable.h"

#include <memory>
#include <string>
#include <unordered_map>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <json/json.hpp>

#include "../../engine/graphics/texture/Texture.h"
#include "../../engine/model/Model.h"
#include "Moon.h"
#include "Rings.h"

class Planet
	: private INoncopyable, private INonmovable
{
private:
	const Model& m_model;
	std::unique_ptr<Texture> m_texture = nullptr;
	std::unique_ptr<Rings> m_rings = nullptr;
	std::unordered_map<std::string, std::unique_ptr<Moon>> m_moons;

	glm::mat4 m_modelMatrix{ 1.0f };

	float m_orbitAngle = 0.0f;
	float m_rotationAngle = 0.0f;
	glm::vec3 m_position{ 0.0f };

	const std::string m_name;
	const std::string m_ringsName;

	const float m_distanceFromSun;
	const float m_orbitVelocity;
	const float m_orbitInclination;

	const float m_rotationVelocity;
	const float m_axialTilt;

	const glm::vec3 m_scale;

public:
	Planet(const class Renderer& renderer, const Model& model, const std::string& name, const nlohmann::json& data);
	~Planet() noexcept = default;

	void Update(const float deltaTime);
	void Draw(class Renderer& renderer, const AssetManager<class GraphicsPipeline>& pipelines) const;
	void DrawRings(class Renderer& renderer, const AssetManager<GraphicsPipeline>& pipelines) const;

	inline const glm::mat4& GetModelMatrix() const noexcept { return m_modelMatrix; }
	inline const Texture& GetTexture() const noexcept { return *m_texture; }
	inline const Texture& GetRingsTexture() const noexcept { return m_rings->GetTexture(); }
	inline const Texture& GetMoonTexture(const std::string& moonName) const noexcept { return m_moons.at(moonName)->GetTexture(); }

	inline bool HasRings() const noexcept { return m_rings != nullptr; }

	inline const glm::vec3& GetPosition() const noexcept { return m_position; }
	inline float GetOrbitAngle() const noexcept { return m_orbitAngle; }

private:
	void CreateRings(const class Renderer& renderer, const nlohmann::json& ringData);
	void PopulateMoons(const class Renderer& renderer, const nlohmann::json& moonData);
};