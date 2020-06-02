#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

template <typename T>
class AssetManager
{
private:
	std::unordered_map<std::string, std::unique_ptr<T>> m_assets;

public:
	AssetManager() = default;
	~AssetManager() noexcept = default;

	template <typename... Args, typename = std::enable_if_t<std::is_constructible_v<T, Args...>>>
	void Add(const std::string& name, Args&&... args)
	{
		m_assets[name] = std::make_unique<T>(std::forward<Args>(args)...);
	}

	void Remove(const std::string& name);

	inline const T& Get(const std::string& name) const { return *m_assets.at(name); }
	inline const T& operator [](const std::string& name) const { return Get(name); }

	inline T& Get(const std::string& name) { return *m_assets.at(name); }
	inline T& operator [](const std::string& name) { return Get(name); }

	inline decltype(m_assets)& GetAssets() { return m_assets; }
	inline const decltype(m_assets)& GetAssets() const { return m_assets; }
};