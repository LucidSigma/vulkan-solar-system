#include "Camera3D.h"

#include <algorithm>
#include <limits>

#include <SDL2/SDL.h>

Camera3D::Camera3D(const glm::vec3& position, const glm::vec3& up, const float yaw, const float pitch)
	: m_position(position), m_worldUp(up), m_yaw(yaw), m_pitch(pitch)
{
	UpdateVectors();
}

void Camera3D::ProcessInput(const input::KeyboardState& keyboardState, const input::MouseState& mouseState, const std::int32_t scrollState)
{
	m_velocity = glm::vec3{ 0.0f, 0.0f, 0.0f };
	float movementSpeed = s_DefaultSpeed;
	
	if (keyboardState.GetKey(SDL_SCANCODE_LSHIFT))
	{
		movementSpeed *= s_SpeedMultiplier;
	}
	else if (keyboardState.GetKey(SDL_SCANCODE_LALT))
	{
		movementSpeed /= s_SpeedMultiplier;
	}

	if (keyboardState.GetKey(SDL_SCANCODE_W))
	{
		m_velocity += m_front * movementSpeed;
	}

	if (keyboardState.GetKey(SDL_SCANCODE_S))
	{
		m_velocity -= m_front * movementSpeed;
	}

	if (keyboardState.GetKey(SDL_SCANCODE_A))
	{
		m_velocity += m_right * movementSpeed;
	}

	if (keyboardState.GetKey(SDL_SCANCODE_D))
	{
		m_velocity -= m_right * movementSpeed;
	}

	if (keyboardState.GetKey(SDL_SCANCODE_SPACE))
	{
		m_velocity.y += movementSpeed;
	}

	if (keyboardState.GetKey(SDL_SCANCODE_LCTRL))
	{
		m_velocity.y -= movementSpeed;
	}

	if (keyboardState.GetKeyDown(SDL_SCANCODE_C))
	{
		m_zoom = s_EnhancedZoom;
		m_hasZoomChanged = true;
	}
	else if (keyboardState.GetKeyUp(SDL_SCANCODE_C))
	{
		m_zoom = s_DefaultZoom;
		m_hasZoomChanged = true;
	}

	const float xOffset = mouseState.relativeX * m_mouseSensitivity;
	const float yOffset = mouseState.relativeY * m_mouseSensitivity;
	m_yaw -= xOffset;
	m_pitch -= yOffset;

	constexpr float MaxPitch = 89.5f;
	constexpr float MinPitch = -89.5f;
	m_pitch = std::clamp(m_pitch, MinPitch, MaxPitch);

	UpdateVectors();
}

void Camera3D::Update(const float deltaTime)
{
	m_position += m_velocity * deltaTime;

	if (m_hasZoomChanged)
	{
		m_hasZoomChanged = false;
	}
}

void Camera3D::UpdateVectors()
{
	glm::vec3 updatedFront{
		glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch)),
		glm::sin(glm::radians(m_pitch)),
		glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch))
	};

	m_front = glm::normalize(updatedFront);
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}