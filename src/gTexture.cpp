#include "gTexture.h"

#include "textureMgr.h"

namespace
{
    DEFINE_GADGET_PROPERTY(gTexture, Name, std::string)
    DEFINE_GADGET_PROPERTY(gTexture, SdlRectX, int)
    DEFINE_GADGET_PROPERTY(gTexture, SdlRectY, int)
    DEFINE_GADGET_PROPERTY(gTexture, SdlRectW, int)
    DEFINE_GADGET_PROPERTY(gTexture, SdlRectH, int)
}

DEFINE_GADGET_API(gTexture)
{
    REGISTER_GADGET_PROPERTY(Name),
    REGISTER_GADGET_PROPERTY(SdlRectX),
    REGISTER_GADGET_PROPERTY(SdlRectY),
    REGISTER_GADGET_PROPERTY(SdlRectW),
    REGISTER_GADGET_PROPERTY(SdlRectH),
    {0, 0}
};

DEFINE_GADGET_CLASS(gTexture)

gTexture::gTexture():
    iSdlRectX(0),
    iSdlRectY(0),
    iSdlRectW(800),
    iSdlRectH(600),
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
    if (iIsEnabled) {
	SDL_Rect rect;
	rect.x = iSdlRectX;
	rect.y = iSdlRectY;
	rect.w = iSdlRectW;
	rect.h = iSdlRectH;
	SDL_RenderCopy(aRenderer, iTexture, 0, &rect);
    }
}

void gTexture::begin()
{
    iTexture = eTextureMgr::getMe()->getTexture(iName);
}
