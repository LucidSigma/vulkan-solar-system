#pragma once

#include "../engine/utility/interfaces/INoncopyable.h"
#include "../engine/utility/interfaces/INonmovable.h"

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "../engine/camera/Camera3D.h"
#include "../engine/graphics/texture/Cubemap.h"
#include "../engine/graphics/buffers/IndexBuffer.h"
#include "../engine/graphics/buffers/VertexBuffer.h"

class Skybox
	: private INoncopyable, private INonmovable
{
private:
	VertexBuffer m_vertices;
	IndexBuffer m_indices;

	std::unique_ptr<Cubemap> m_cubemap;

public:
	Skybox(const class Renderer& renderer, const std::string& folderPath, const std::string& imageExtension);
	~Skybox() noexcept = default;

	void Draw(class Renderer& renderer, const class GraphicsPipeline& pipeline) const;

	inline const Cubemap& GetCubemap() const noexcept { return *m_cubemap; }
	inline glm::mat4 GetModelViewMatrix(const Camera3D& camera) const noexcept { return glm::mat4(glm::mat3(camera.GetViewMatrix())); }

private:
	void InitialiseBuffers();
	void InitialiseCubemap(const class Renderer& renderer, const std::string& folderPath, const std::string& imageExtension);
};