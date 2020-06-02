#pragma once

#include <string_view>

#include <json/json.hpp>

class Config
{
private:
	nlohmann::json m_data;

public:
	Config() = default;
	~Config() noexcept = default;

	void Initialse(const std::string_view& configFilepath);

	inline const auto& operator [](const std::string_view& configType) const { return m_data[configType.data()]; }
};