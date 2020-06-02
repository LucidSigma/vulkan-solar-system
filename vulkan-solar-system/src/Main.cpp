#include <cstdlib>
#include <exception>

#include "engine/Application.h"
#include "solar_system/SolarSystem.h"

int main(const int argc, char* argv[])
try
{
	Application application;
	application.GetSceneManager().PushScene<SolarSystem>(application);
	application.Run();

	return EXIT_SUCCESS;
}
catch (const std::exception& error)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", error.what(), nullptr);

	return EXIT_FAILURE;
}