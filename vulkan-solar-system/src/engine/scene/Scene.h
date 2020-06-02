#pragma once

#include "../utility/interfaces/INoncopyable.h"
#include "../utility/interfaces/INonmovable.h"

#include <cstdint>

#include <SDL2/SDL.h>

#include "../input/InputStates.h"

class Scene
	: private INoncopyable, private INonmovable
{
protected:
	class Application& m_application;

public:
	explicit Scene(class Application& application);
	virtual ~Scene() noexcept = default;

	virtual void OnLoad() = 0;
	virtual void OnUnload() noexcept = 0;

	virtual void PollEvents(const SDL_Event& event) = 0;
	virtual void ProcessInput(const input::KeyboardState& keyboardState, const input::MouseState& mouseState, const std::int32_t scrollState) = 0;
	virtual void Update(const float deltaTime) = 0;
	virtual void Render(class Renderer& renderer) const = 0;

	inline virtual void ProcessWindowResize() { }
};