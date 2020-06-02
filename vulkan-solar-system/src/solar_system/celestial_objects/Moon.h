#pragma once

#include "../../engine/utility/interfaces/INoncopyable.h"
#include "../../engine/utility/interfaces/INonmovable.h"

#include <memory>
#include <string>
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <json/json.hpp>

#include "../../engine/graphics/texture/Texture.h"
#include "../../engine/model/Model.h"

class Moon
	: private INoncopyable, private INonmovable
{
private:
	const Model& m_model;	
	std::unique_ptr<Texture> m_texture = nullptr;

	glm::mat4 m_modelMatrix{ 1.0f };

	float m_orbitAngle = 0.0f;
	float m_rotationAngle = 0.0f;

	const float m_distanceFromPlanet;
	const float m_orbitVelocity;
	const float m_orbitInclination;

	const float m_rotationVelocity;
	const float m_axialTilt;

	const glm::vec3 m_scale;

public:
	Moon(const class Renderer& renderer, const Model& model, const std::string& planetName, const std::string& name, const nlohmann::json& data);
	~Moon() noexcept = default;

	void Update(const float deltaTime);
	void Draw(class Renderer& renderer) const;

	inline const glm::mat4& GetModelMatrix() const noexcept { return m_modelMatrix; }
	inline void SetModelMatrix(const glm::mat4& modelMatrix) noexcept { m_modelMatrix = modelMatrix; }

	inline const Texture& GetTexture() const noexcept { return *m_texture; }
};