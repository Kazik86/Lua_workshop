#include "renderer.h"

#include "actorMgr.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

eRenderable::eRenderable():
    iZOrder(0)
{
    eRenderer::getMe()->incRenderables();
}

void eRenderable::addToRenderingQueue()
{
    eRenderer::getMe()->addRenderable(this);
}

///////////////////////////////////////////////////////////////////////

eRenderer* eRenderer::iMe = 0;

eRenderer::eRenderer():
    iWindow(0),
    iRenderer(0),
    iRenderables(eActorMgr::EActorsCapacity),
    iRenderablesZOrder(16),
    iRenderablesNum(0),
    iFrameCntr(0)
{
    if (iMe)
	throw std::runtime_error("eRenderer: multiple instances not allowed.");

    iMe = this;
}

eRenderer::~eRenderer()
{
    SDL_DestroyRenderer(iRenderer);
    SDL_DestroyWindow(iWindow);
    SDL_Quit();
    iMe = 0;
}

void eRenderer::init(const std::string& aCaption, int aX, int aY, int aWidth, int aHeight, int aFlags)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
	std::string msg = "SDL_Init: ";
	msg += SDL_GetError();
	throw std::runtime_error(msg);
    }

    iWindow = SDL_CreateWindow(aCaption.c_str(), aX, aY, aWidth, aHeight, aFlags);

    if (iWindow == 0) {
	std::string msg = "SDL_CreateWindow: ";
	msg += SDL_GetError();
	throw std::runtime_error(msg);
    }

    iRenderer = SDL_CreateRenderer(iWindow, -1, 0);

    if (iRenderer == 0) {
	std::string msg = "SDL_CreateRenderer: ";
	msg += SDL_GetError();
	throw std::runtime_error(msg);
    }

    /*
    SDL_SetRenderDrawColor(iRenderer, 0, 0, 0, 0);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(iRenderer, aWidth, aHeight);
    */
}

void eRenderer::draw()
{
    for (eRenderable* r : iRenderables)
        r->draw();
}

void eRenderable::renderCopyEx(const std::vector<eRenderable*>& aCollection) const
{
    for (eRenderable* r : aCollection) {
        for (const sRenderCopyEx& rce : r->iRCE)
            SDL_RenderCopyEx(iRenderer, rce->iTexture, 0, &r->iSdlRect, r->iAngle, NULL, SDL_FLIP_NONE);
    }
}

void eRenderer::render(float aDelta)
{
    ++iFrameCntr;
    SDL_RenderClear(iRenderer);

    renderCopyEx(iRenderables);

    std::sort(iRenderablesZOrder.begin(), iRenderablesZOrder.end(), [](eRenderable* aLhs, eRenderable* aRhs) {
            return aLhs->iZOrder < aRhs->iZOrder;
    });

    renderCopyEx(iRenderablesZOrder);
    
    SDL_RenderPresent(iRenderer);
}

void eRenderer::addRenderable(eRenderable* aObj)
{
    aObj->iZOrder ? iRenderablesZOrder.push_back(aObj) : iRenderables.push_back(aObj);
}

void eRenderer::incRenderables()
{
    ++iRenderablesNum;

    if (iRenderablesNum > eActorMgr::EActorsCapacity)
        std::cout << "WARNING: eRenderer::iRenderablesNum > eActorMgr::EActorsCapacity" << std::endl;
}
