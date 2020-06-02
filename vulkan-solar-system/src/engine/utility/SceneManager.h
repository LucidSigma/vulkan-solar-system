#pragma once

#include <memory>
#include <queue>
#include <type_traits>
#include <utility>

#include "../scene/Scene.h"

class SceneManager
{
private:
	std::queue<std::unique_ptr<Scene>> m_scenes;

public:
	SceneManager() = default;
	~SceneManager() noexcept = default;

	template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Scene, T>&& std::is_constructible_v<T, Args...>>>
	void PushScene(Args&&... args)
	{
		m_scenes.push(std::make_unique<T>(std::forward<Args>(args)...));
	}

	void PopScene();

	inline const std::unique_ptr<Scene>& CurrentScene() const noexcept { return m_scenes.front(); }
	inline bool IsEmpty() const noexcept { return m_scenes.empty(); }
};