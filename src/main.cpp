#include "game.h"

#include <SDL2/SDL.h>
#include <stdexcept>

int main(int /* aArgc */, char** /* aArgv */)
{
    int result = EXIT_SUCCESS;

    try {
        sGameProps gameProps;
	eGame game(gameProps);
	game.mainLoop();
    } catch (const std::exception& aError) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, aError.what());
	eGame::cleanup();
	result = EXIT_FAILURE;
    }

    return result;
}
