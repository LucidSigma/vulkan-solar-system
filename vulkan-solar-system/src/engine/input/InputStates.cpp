#include "InputStates.h"

#include <type_traits>

bool input::KeyboardState::GetKeyDown(const SDL_Scancode key) const
{
	return currentKeys[key] && !previousKeys[key];
}

bool input::KeyboardState::GetKey(const SDL_Scancode key) const
{
	return currentKeys[key];
}

bool input::KeyboardState::GetKeyUp(const SDL_Scancode key) const
{
	return !currentKeys[key] && previousKeys[key];
}

bool input::MouseState::GetButtonDown(const MouseButton button) const
{
	const auto buttonValue = static_cast<std::underlying_type_t<MouseButton>>(button);

	return (currentButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue) && !((previousButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue));
}

bool input::MouseState::GetButton(const MouseButton button) const
{
	const auto buttonValue = static_cast<std::underlying_type_t<MouseButton>>(button);

	return (currentButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue);
}

bool input::MouseState::GetButtonUp(const MouseButton button) const
{
	const auto buttonValue = static_cast<std::underlying_type_t<MouseButton>>(button);

	return !((currentButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue)) && (previousButtonStates & SDL_BUTTON(buttonValue)) == SDL_BUTTON(buttonValue);
}