#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../input/InputStates.h"

class Camera3D final
{
private:
	static constexpr float s_DefaultYaw = 90.0f;
	static constexpr float s_DefaultPitch = 0.0f;
	
	static constexpr float s_DefaultSpeed = 100.0f;
	static constexpr float s_SpeedMultiplier = 16.0f;
	static constexpr float s_DefaultSensitivity = 0.1f;

	static constexpr float s_DefaultZoom = 45.0f;
	static constexpr float s_EnhancedZoom = 10.0f;

	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_front{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
	glm::vec3 m_right{ 0.0f, 0.0f, 0.0f };
	glm::vec3 m_worldUp{ 0.0f, 1.0f, 0.0f };

	glm::vec3 m_velocity{ 0.0f, 0.0f, 0.0f };

	float m_yaw;
	float m_pitch;

	float m_mouseSensitivity = s_DefaultSensitivity;
	float m_zoom = s_DefaultZoom;
	bool m_hasZoomChanged = false;

public:
	Camera3D(const glm::vec3& position = glm::vec3{ 0.0f, 0.0f, 0.0f }, const glm::vec3& up = glm::vec3{ 0.0f, 1.0f, 0.0f }, const float yaw = s_DefaultYaw, const float pitch = s_DefaultPitch);
	~Camera3D() noexcept = default;

	void ProcessInput(const input::KeyboardState& keyboardState, const input::MouseState& mouseState, const std::int32_t scrollState);
	void Update(const float deltaTime);

	inline float GetZoom() const noexcept { return m_zoom; }
	inline bool HasZoomChanged() const noexcept { return m_hasZoomChanged; }

	inline const glm::vec3& GetPosition() const noexcept { return m_position; }
	inline void SetPosition(const glm::vec3& position) noexcept { m_position = position; }

	inline const glm::vec3& GetFront() const noexcept { return m_front; }
	inline glm::mat4 GetViewMatrix() const noexcept { return glm::lookAtLH(m_position, m_position + m_front, m_up); }

private:
	void UpdateVectors();
};