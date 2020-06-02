#pragma once

#include <cstdint>

#include <SDL2/SDL.h>

namespace input
{
	struct KeyboardState
	{
		const std::uint8_t* currentKeys;
		const std::uint8_t* previousKeys;

		bool GetKeyDown(const SDL_Scancode key) const;
		bool GetKey(const SDL_Scancode key) const;
		bool GetKeyUp(const SDL_Scancode key) const;
	};
	
	struct MouseState
	{
		enum class MouseButton
			: decltype(SDL_BUTTON_LEFT)
		{
			Left = SDL_BUTTON_LEFT,
			Middle = SDL_BUTTON_MIDDLE,
			Right = SDL_BUTTON_RIGHT,
			Thumb1 = SDL_BUTTON_X1,
			Thumb2 = SDL_BUTTON_X2
		};

		std::uint32_t currentButtonStates;
		std::uint32_t previousButtonStates;

		float x;
		float y;

		float relativeX;
		float relativeY;

		bool GetButtonDown(const MouseButton button) const;
		bool GetButton(const MouseButton button) const;
		bool GetButtonUp(const MouseButton button) const;
	};
}