#include "Application.h"

#include <stdexcept>
#include <string>

#include "input/InputStates.h"

Application::Application()
{
	Initialise();
}

Application::~Application() noexcept
{
	m_renderer = nullptr;
	m_window = nullptr;

	SDL_Quit();
}

void Application::Run()
{
	m_isRunning = !m_sceneManager.IsEmpty();
	SDL_Event event{ };

	if (m_isRunning)
	{
		m_sceneManager.CurrentScene()->OnLoad();
	}

	while (m_isRunning)
	{
		PollEvents(event);
		ProcessInput();
		Update();
		Render();

		UpdateSceneQueue();
	}

	m_renderer->FinaliseRenderOperations();
}

void Application::FinishCurrentScene()
{
	m_isCurrentSceneFinished = true;
}

void Application::Initialise()
{
	m_config.Initialse("config/config.json");
	constexpr int SDLSuccess = 0;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != SDLSuccess)
	{
		throw std::runtime_error("Failed to initialise SDL.");
	}

	SDL_setenv("SDL_VIDEO_MINIMIZE_ON_FOCUS_LOSS", m_config["enable-fullscreen-minimise"] ? "1" : "0", SDL_TRUE);

	m_window = std::make_unique<Window>(m_config["window"]["title"], glm::uvec2{ m_config["window"]["size"]["width"], m_config["window"]["size"]["height"] }, m_config["window"]["start-as-fullscreen"]);
	m_window->SetIcon("assets/icon/icon.png");

	m_renderer = std::make_unique<Renderer>(*m_window, Renderer::Config{
		.enableVsync = m_config["enable-vsync"],
		.enableMultisample = m_config["enable-multisample"]
	});

	if (SDL_CaptureMouse(SDL_TRUE) != SDLSuccess)
	{
		throw std::runtime_error("Failed to capture mouse cursor.");
	}

	if (SDL_SetRelativeMouseMode(SDL_TRUE) != SDLSuccess)
	{
		throw std::runtime_error("Failed to switch to relative mouse mode.");
	}
	
	SDL_GetRelativeMouseState(nullptr, nullptr);
}

void Application::PollEvents(SDL_Event& event)
{
	m_inputManager.SetScrollAmount(0);

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_F11:
				m_window->ToggleFullscreen();

				break;

			default:
				break;
			}

		case SDL_MOUSEWHEEL:
			m_inputManager.SetScrollAmount(event.wheel.y);

			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				m_window->ProcessResize(glm::uvec2{ event.window.data1, event.window.data2 });

				[[fallthrough]];

			case SDL_WINDOWEVENT_MINIMIZED:
				m_renderer->ProcessWindowResize();
				m_sceneManager.CurrentScene()->ProcessWindowResize();

				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
				m_hasFocus = false;

				break;

			case SDL_WINDOWEVENT_FOCUS_GAINED:
				m_hasFocus = true;

				break;

			default:
				break;
			}

			break;
		
		case SDL_QUIT:
			m_isRunning = false;

			break;

		default:
			break;
		}

		m_sceneManager.CurrentScene()->PollEvents(event);
	}
}

void Application::ProcessInput()
{
	const input::KeyboardState keyboardState = m_inputManager.GetKeyboardState();
	const input::MouseState mouseState = m_inputManager.GetMouseState();
	const std::int32_t scrollState = m_inputManager.GetScrollState();

	m_sceneManager.CurrentScene()->ProcessInput(keyboardState, mouseState, scrollState);
}

void Application::Update()
{
	const float deltaTime = CalculateDeltaTime();

	if (m_hasFocus)
	{
		m_sceneManager.CurrentScene()->Update(deltaTime);
	}
}

void Application::Render() const
{
	if (m_hasFocus)
	{
		if (m_renderer->PrepareRender())
		{
			m_renderer->BeginRender();
			{
				m_sceneManager.CurrentScene()->Render(*m_renderer);
			}
			m_renderer->EndRender();

			m_renderer->Present();
		}
	}
}

void Application::UpdateSceneQueue()
{
	if (m_isCurrentSceneFinished)
	{
		m_renderer->FinaliseRenderOperations();

		m_sceneManager.CurrentScene()->OnUnload();
		m_sceneManager.PopScene();

		if (!m_sceneManager.IsEmpty())
		{
			m_sceneManager.CurrentScene()->OnLoad();
		}

		m_isCurrentSceneFinished = false;
	}

	if (m_sceneManager.IsEmpty())
	{
		m_isRunning = false;
	}
}

float Application::CalculateDeltaTime()
{
	constexpr float MillisecondsPerSecond = 1'000.0f;

	const float deltaTime = static_cast<float>(SDL_GetTicks() - m_ticksCount) / MillisecondsPerSecond;
	m_ticksCount = SDL_GetTicks();

	return deltaTime;
}