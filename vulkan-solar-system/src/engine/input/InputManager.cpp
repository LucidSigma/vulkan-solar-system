#include "InputManager.h"

#include <utility>

input::KeyboardState InputManager::GetKeyboardState()
{
	m_previousKeys = std::move(m_currentKeys);
	m_currentKeys = std::move(std::vector<std::uint8_t>(SDL_GetKeyboardState(nullptr), SDL_GetKeyboardState(nullptr) + SDL_NUM_SCANCODES));

	return input::KeyboardState{
		m_currentKeys.data(),
		m_previousKeys.data()
	};
}

input::MouseState InputManager::GetMouseState()
{
	int x = 0;
	int y = 0;
	m_previousMouseButtonStates = m_currentMouseButtonStates;
	m_currentMouseButtonStates = SDL_GetMouseState(&x, &y);

	int relativeX = 0;
	int relativeY = 0;
	SDL_GetRelativeMouseState(&relativeX, &relativeY);

	return input::MouseState{
		m_currentMouseButtonStates,
		m_previousMouseButtonStates,
		static_cast<float>(x),
		static_cast<float>(y),
		static_cast<float>(relativeX),
		static_cast<float>(relativeY)
	};
}

std::int32_t InputManager::GetScrollState() const
{
	return m_yScrollAmount;
}