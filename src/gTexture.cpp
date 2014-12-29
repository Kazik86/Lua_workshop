#include "gTexture.h"

#include "textureMgr.h"

DEFINE_GADGET_PROPERTY(gTexture, Name, iName)
DEFINE_GADGET_PROPERTY(gTexture, SdlRectX, iSdlRect.x)
DEFINE_GADGET_PROPERTY(gTexture, SdlRectY, iSdlRect.y)
DEFINE_GADGET_PROPERTY(gTexture, SdlRectW, iSdlRect.w)
DEFINE_GADGET_PROPERTY(gTexture, SdlRectH, iSdlRect.h)

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
	SDL_RenderCopy(aRenderer, iTexture, 0, &iSdlRect);
    }
}

void gTexture::begin()
{
    iTexture = eTextureMgr::getMe()->getTexture(iName);
}
