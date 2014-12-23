#include "gTexture.h"

#include "textureMgr.h"

namespace
{
    DEFINE_GADGET_PROPERTY(gTexture, Name, std::string)
}

DEFINE_GADGET_API(gTexture)
{
    REGISTER_GADGET_PROPERTY(Name),
    {0, 0}
};

DEFINE_GADGET_CLASS(gTexture)

gTexture::gTexture():
    iTexture(0)
{

}

gTexture::~gTexture()
{

}

void gTexture::update(lua_State* /* aLua */, float /* aDelta */)
{
}

void gTexture::draw(SDL_Renderer* aRenderer)
{
    if (iIsEnabled)
	SDL_RenderCopy(aRenderer, iTexture, 0, 0);
}

void gTexture::begin()
{
    iTexture = eTextureMgr::getMe()->getTexture(iName);
}
