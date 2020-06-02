#include "Config.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

void Config::Initialse(const std::string_view& configFilepath)
{
	std::ifstream configFile(configFilepath.data());

	if (!configFile.is_open())
	{
		throw std::runtime_error("Failed to open config file.");
	}

	std::ostringstream configStringStream;
	configStringStream << configFile.rdbuf();
	configFile.close();

	m_data = nlohmann::json::parse(configStringStream.str());
}