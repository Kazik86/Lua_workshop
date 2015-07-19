#include "gTexture.h"

#include "textureMgr.h"

#include <glm/glm.hpp>

DEFINE_GADGET_ACCESSOR(gTexture, getName, setName, iName)
DEFINE_GADGET_ACCESSOR(gTexture, getSdlRectX, setSdlRectX, iSdlRect.x)
DEFINE_GADGET_ACCESSOR(gTexture, getSdlRectY, setSdlRectY, iSdlRect.y)
DEFINE_GADGET_ACCESSOR(gTexture, getSdlRectW, setSdlRectW, iSdlRect.w)
DEFINE_GADGET_ACCESSOR(gTexture, getSdlRectH, setSdlRectH, iSdlRect.h)
DEFINE_GADGET_WRITER(gTexture, setPosFromActor, iPosFromActor)

DEFINE_GADGET_API(gTexture)
{
    REGISTER_GADGET_ACCESSOR(getName, setName)
    REGISTER_GADGET_ACCESSOR(getSdlRectX, setSdlRectX)
    REGISTER_GADGET_ACCESSOR(getSdlRectY, setSdlRectY)
    REGISTER_GADGET_ACCESSOR(getSdlRectW, setSdlRectW)
    REGISTER_GADGET_ACCESSOR(getSdlRectH, setSdlRectH)
    REGISTER_GADGET_WRITER(setPosFromActor)
    {0, 0}
};

DEFINE_GADGET_CLASS(gTexture)

gTexture::gTexture():
    iTexture(0),
    iPosFromActor(true)
{

}

gTexture::~gTexture()
{

}

int gTexture::update(lua_State* /* aLua */, float /* aDelta */)
{
    if (iPosFromActor) {
	const glm::vec2& pos = getActor()->getPos();
	iSdlRect.x = pos.x;
	iSdlRect.y = pos.y;
    }

    return 0;
}

void gTexture::draw(SDL_Renderer* aRenderer, float /* aDelta */)
{
    if (iIsEnabled) {
	SDL_RenderCopy(aRenderer, iTexture, 0, &iSdlRect);
    }
}

void gTexture::begin()
{
    iTexture = eTextureMgr::getMe()->getTexture(iName);
}
