#pragma once

#include "utility/interfaces/INoncopyable.h"
#include "utility/interfaces/INonmovable.h"

#include <cstdint>

#include <json/json.hpp>

#include "graphics/renderer/Renderer.h"
#include "input/InputManager.h"
#include "scene/Scene.h"
#include "utility/Config.h"
#include "utility/SceneManager.h"
#include "window/Window.h"

class Application final
	: private INoncopyable, private INonmovable
{
private:
	std::unique_ptr<Window> m_window = nullptr;
	std::unique_ptr<Renderer> m_renderer = nullptr;
	bool m_hasFocus = true;

	bool m_isRunning = true;
	std::uint64_t m_ticksCount = 0;

	InputManager m_inputManager;
	Config m_config;

	SceneManager m_sceneManager;
	bool m_isCurrentSceneFinished = false;

public:
	Application();
	~Application() noexcept;

	void Run();
	void FinishCurrentScene();

	inline const Window& GetWindow() const noexcept { return *m_window; }
	inline Renderer& GetRenderer() noexcept { return *m_renderer; }
	inline const Config& GetConfig() const noexcept { return m_config; }
	inline SceneManager& GetSceneManager() noexcept { return m_sceneManager; }

private:
	void Initialise();

	void PollEvents(SDL_Event& event);
	void ProcessInput();
	void Update();
	void Render() const;

	void UpdateSceneQueue();

	float CalculateDeltaTime();
};