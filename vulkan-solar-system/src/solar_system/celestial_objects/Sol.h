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

class Sol
	: private INoncopyable, private INonmovable
{
private:
	std::unique_ptr<Model> m_model = nullptr;
	std::unique_ptr<Texture> m_texture = nullptr;

	glm::mat4 m_modelMatrix{ 1.0f };

	float m_rotationAngle = 0.0f;

	const std::string m_name;
	const float m_rotationVelocity;
	const glm::vec3 m_scale;

public:
	Sol(const class Renderer& renderer, const std::string& name, const nlohmann::json& data);
	~Sol() noexcept = default;

	void Update(const float deltaTime);
	void Draw(class Renderer& renderer, const AssetManager<class GraphicsPipeline>& pipelines) const;
	void DrawGlow(class Renderer& renderer, const class GraphicsPipeline& pipeline) const;

	inline const glm::mat4& GetModelMatrix() const noexcept { return m_modelMatrix; }
	inline const Texture& GetTexture() const noexcept { return *m_texture; }
};