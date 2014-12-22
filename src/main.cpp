#include "game.h"

#include <SDL2/SDL.h>
#include <stdexcept>

int main(int /* aArgc */, char** /* aArgv */)
{
    int result = EXIT_SUCCESS;

    try {
	eGame game;
	game.mainLoop();
    } catch (const std::exception& aError) {
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, aError.what());
	result = EXIT_FAILURE;
    }

    return result;
}
