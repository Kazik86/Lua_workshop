#include "game.h"

#include "actorMgr.h"
#include "luaState.h"

#include <SDL2/SDL.h>
#include <stdexcept>

eGame* eGame::iMe = 0;

eGame::eGame():
    iIsRunning(true),
    iLua(new eLuaState()),
    iActorMgr(new eActorMgr())
{
    if (iMe)
	throw std::runtime_error("eGame: multiple instances not allowed.");

    iMe = this;

    if (SDL_Init(0/*SDL_INIT_VIDEO|SDL_INIT_TIMER*/) != 0) {
	throw std::runtime_error(SDL_GetError());
    }

    eActorMgr* actorMgr = eActorMgr::getMe();
    // actorMgr->add(iLua.get(), "scripts.foo");
    // actorMgr->add(iLua.get(), "scripts.foo");
    actorMgr->doScript(iLua->getRaw());
}

eGame::~eGame()
{
    SDL_Quit();
    iMe = 0;
}

void eGame::mainLoop()
{
    while (iIsRunning) {
	handleEvents();
	iActorMgr->update(iLua.get()->getRaw());
    }
}

void eGame::handleEvents()
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
