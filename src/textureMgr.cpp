#include "textureMgr.h"

#include "renderer.h"

#include <iostream>
#include <SDL2/SDL_image.h>
#include <stdexcept>

eTextureMgr* eTextureMgr::iMe = 0;

eTextureMgr::eTextureMgr()
{
    if (iMe)
	throw std::runtime_error("eTextureMgr: multiple instances not allowed.");

    iMe = this;
}

eTextureMgr::~eTextureMgr()
{
    for (auto& p : iTextures)
	SDL_DestroyTexture(p.second);

    iTextures.clear();
    IMG_Quit();
    iMe = 0;
}

void eTextureMgr::init()
{
    int imgFlags = IMG_INIT_PNG;
    int imgStatus = IMG_Init(imgFlags);

    if (imgStatus != imgFlags)
	throw std::runtime_error("eTextureMgr::init::IMG_Init: Could't initialize PNG support.\n");
}

SDL_Texture* eTextureMgr::getTexture(const std::string& aName)
{
    SDL_Texture* t = 0;
    auto f = iTextures.find(aName);

    if (f == iTextures.end())
    {
	SDL_Surface* tmp = IMG_Load(aName.c_str());

	if (tmp) {
	    t = SDL_CreateTextureFromSurface(eRenderer::getMe()->getRaw(), tmp);
	    iTextures.insert(f, std::make_pair(aName, t));
	    SDL_FreeSurface(tmp);
	} else {
	    std::string msg = "eTextureMgr::getTexture::IMG_Load: ";
	    msg += IMG_GetError();
	    msg += "\n";
	    std::cerr << msg;
	}
    }
    else
	t = f->second;

    return t;
}
