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

class Rings
	: private INoncopyable, private INonmovable
{
private:
	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;

	std::unique_ptr<Texture> m_texture = nullptr;

	glm::mat4 m_model{ 1.0f };

	const std::string m_name;

	float m_rotationAngle = 0.0f;
	const float m_rotationVelocity;
	const float m_inclination;
	const float m_scale;

public:
	Rings(const class Renderer& renderer, const std::string& planetName, const nlohmann::json& data);
	~Rings() noexcept = default;

	void Update(const float deltaTime);
	void Draw(class Renderer& renderer) const;

	inline const Texture& GetTexture() const noexcept { return *m_texture; }

	inline const glm::mat4& GetModelMatrix() const noexcept { return m_model; }
	inline void SetModelMatrix(const glm::mat4& model) noexcept { m_model = model; }

private:
	void InitialiseBuffers();
	void InitialiseTexture(const class Renderer& renderer, const std::string& planetName, const std::string& imageExtension);
};