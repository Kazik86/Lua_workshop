#include "game.h"

#include "actorMgr.h"
#include "fontMgr.h"
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
    eFontMgr iFontMgr;
    eTextureMgr iTextureMgr;
};

eGame* eGame::iMe = 0;
sGameResources* eGame::iResources = 0;

eGame::eGame(const sGameProps& aProps):
    iGameProps(aProps),
#ifdef KPR_REAL_TIME_UPDATE
    iRtu("/tmp/rtu"),
#endif
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
    eActorMgr::getMe()->init();
    eRenderer::getMe()->init("Tanki", 0, 0, 800, 600, 0);
    eFontMgr::getMe()->init();
    eTextureMgr::getMe()->init();

    // actor "Main"
    eActorMgr::getMe()->add(iResources->iLua.getRaw(), iGameProps.iMainActorScript, 0);
}

eGame::~eGame()
{
    cleanup();
}

void eGame::cleanup()
{
    delete iResources;
    iResources = 0;
    iMe = 0;
}

void eGame::mainLoop()
{
    unsigned int current;
    int dt;
    eRenderer* renderer = eRenderer::getMe();

    while (iIsRunning) {
	current = SDL_GetTicks();
	dt = current - iLastUpdateTime;
	iLastUpdateTime = current;
	dt = glm::max(0, dt);
	iAccumulator += dt;
	iAccumulator = glm::clamp(iAccumulator, 0, KMaxAccumulatedTime);

	while(iAccumulator > KTimeStep)
	{
	    if (! iPause) {
#ifdef KPR_REAL_TIME_UPDATE
                iRtu.update();
#endif

                renderer->clearRenderingQueue();
		iResources->iActorMgr.update(iResources->iLua.getRaw(), KDelta);
	    }

	    iAccumulator -= KTimeStep;
	}

	renderer->render( float(iAccumulator) / float(1000) );
    }
}

eLuaState* eGame::getLua()
{
    return &(iResources->iLua);
}

