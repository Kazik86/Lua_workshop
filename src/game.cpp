#include "game.h"

#include "actorMgr.h"
#include "luaModule.h"
#include "luaState.h"
#include "renderer.h"
#include "textureMgr.h"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <stdexcept>

namespace
{
    const int KTimeStep = 30;
    const int KMaxAccumulatedTime = 1000;
    const float KDelta = float(KTimeStep) / float(1000);
}

struct sGameResources
{
    // modules
    eLuaState iLua;
    eLuaModuleMgr iModuleMgr;
    eActorMgr iActorMgr;
    eRenderer iRenderer;
    eTextureMgr iTextureMgr;
};

eGame* eGame::iMe = 0;
sGameResources* eGame::iResources = 0;

eGame::eGame():
    iIsRunning(true),
    iAccumulator(0),
    iLastUpdateTime(0),
    iPause(false)
{
    if (iMe)
	throw std::runtime_error("eGame: multiple instances not allowed.");

    iMe = this;

    iResources = new sGameResources();

    // move this to eRenderer ctor?
    eRenderer::getMe()->init("Tanki", 0, 0, 800, 600, 0);
    eTextureMgr::getMe()->init();

    // TODO: the below is only for doing 'doScript' on 'Main' script. Ugly. Get
    // rid of this.
    eActorMgr* actorMgr = eActorMgr::getMe();
    actorMgr->doScript(iResources->iLua.getRaw());
}

eGame::~eGame()
{
    cleanup();
    iMe = 0;
}

void eGame::cleanup()
{
    delete iResources;
    iResources = 0;
}

void eGame::mainLoop()
{
    while (iIsRunning) {
	unsigned int current = SDL_GetTicks();
	int dt = current - iLastUpdateTime;
	iLastUpdateTime = current;
	dt = glm::max(0, dt);
	iAccumulator += dt;
	iAccumulator = glm::clamp(iAccumulator, 0, KMaxAccumulatedTime);

	while(iAccumulator > KTimeStep)
	{
	    if (! iPause) {
		handleEvents();
		iResources->iActorMgr.update(iResources->iLua.getRaw(), KDelta);
	    }

	    iAccumulator -= KTimeStep;
	}

	eRenderer::getMe()->render();
    }
}

void eGame::handleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
	switch (event.type) {
	    case SDL_QUIT:
		iIsRunning = false;
		break;
	    default:
		break;
	}
    }
}

eLuaState* eGame::getLua()
{
    return &(iResources->iLua);
}
