#include "game.h"

#include "actorMgr.h"
#include "luaState.h"

#include <SDL2/SDL.h>
#include <stdexcept>

namespace 
{
    bool iIsRunning = true;
    eLuaState iLua;


    // functions //////////////////////////////////////////////////////

    void initModules()
    {
	ActorMgr::init();
    }

    void cleanupModules()
    {
	ActorMgr::cleanup();
    }

    void handleEvents()
    {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
	    switch (event.type) {
		case SDL_QUIT:
		    iIsRunning = false;
		    break;
		default:
		    break;
	    }
	}
    }
}

void Game::init()
{
    if (SDL_Init(0/*SDL_INIT_VIDEO|SDL_INIT_TIMER*/) != 0) {
	throw std::runtime_error(SDL_GetError());
    }

    initModules();

    ActorMgr::add(iLua, "scripts.foo");
    ActorMgr::add(iLua, "scripts.foo");
    ActorMgr::doScript();
}

void Game::cleanup()
{
    cleanupModules();
    SDL_Quit();
}

void Game::mainLoop()
{
    while (iIsRunning) {
	handleEvents();
	ActorMgr::update();
    }
}

eLuaState& Game::getLua()
{
    return iLua;
}
