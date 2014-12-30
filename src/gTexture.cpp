#include "gTexture.h"

#include "textureMgr.h"

DEFINE_GADGET_ACCESSOR(gTexture, getName, setName, iName)
DEFINE_GADGET_ACCESSOR(gTexture, getSdlRectX, setSdlRectX, iSdlRect.x)
DEFINE_GADGET_ACCESSOR(gTexture, getSdlRectY, setSdlRectY, iSdlRect.y)
DEFINE_GADGET_ACCESSOR(gTexture, getSdlRectW, setSdlRectW, iSdlRect.w)
DEFINE_GADGET_ACCESSOR(gTexture, getSdlRectH, setSdlRectH, iSdlRect.h)

DEFINE_GADGET_API(gTexture)
{
    REGISTER_GADGET_ACCESSOR(getName, setName)
    REGISTER_GADGET_ACCESSOR(getSdlRectX, setSdlRectX)
    REGISTER_GADGET_ACCESSOR(getSdlRectY, setSdlRectY)
    REGISTER_GADGET_ACCESSOR(getSdlRectW, setSdlRectW)
    REGISTER_GADGET_ACCESSOR(getSdlRectH, setSdlRectH)
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
