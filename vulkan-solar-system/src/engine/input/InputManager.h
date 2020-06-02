#pragma once

#include <cstdint>
#include <vector>

#include <SDL2/SDL.h>

#include "InputStates.h"

class InputManager
{
private:
	std::int32_t m_yScrollAmount = 0;

	std::vector<std::uint8_t> m_previousKeys{ std::vector<std::uint8_t>(SDL_NUM_SCANCODES, SDL_FALSE) };
	std::vector<std::uint8_t> m_currentKeys{ std::vector<std::uint8_t>(SDL_NUM_SCANCODES, SDL_FALSE) };

	std::uint32_t m_previousMouseButtonStates = 0;
	std::uint32_t m_currentMouseButtonStates = 0;

public:
	InputManager() = default;
	~InputManager() noexcept = default;

	input::KeyboardState GetKeyboardState();
	input::MouseState GetMouseState();
	std::int32_t GetScrollState() const;

	inline void SetScrollAmount(const std::int32_t scrollAmount) noexcept { m_yScrollAmount = scrollAmount; }
};