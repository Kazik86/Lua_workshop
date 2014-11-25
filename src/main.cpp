#include "game.h"

#include <SDL2/SDL.h>
#include <stdexcept>

int main(int /* aArgc */, char** /* aArgv */)
{
    int result = EXIT_SUCCESS;

    try {
	Game::init();
	Game::mainLoop();
    } catch (const std::exception& aError) {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, aError.what());
	result = EXIT_FAILURE;
    }

    Game::cleanup();

    return result;
}
