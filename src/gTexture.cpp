#include "gTexture.h"

#include "gMove.h"
#include "textureMgr.h"

#include <glm/glm.hpp>
#include <iostream>

DEFINE_GADGET_ACCESSOR(gTexture, getName, setName, iName)
DEFINE_GADGET_ACCESSOR(gTexture, getSdlRectX, setSdlRectX, iSdlRect.x)
DEFINE_GADGET_ACCESSOR(gTexture, getSdlRectY, setSdlRectY, iSdlRect.y)
DEFINE_GADGET_ACCESSOR(gTexture, getSdlRectW, setSdlRectW, iSdlRect.w)
DEFINE_GADGET_ACCESSOR(gTexture, getSdlRectH, setSdlRectH, iSdlRect.h)
DEFINE_GADGET_WRITER(gTexture, setPosFromActor, iPosFromActor)
DEFINE_GADGET_WRITER(gTexture, setMoveGadget, i_gMove)

DEFINE_GADGET_API(gTexture)
{
    REGISTER_GADGET_ACCESSOR(getName, setName)
    REGISTER_GADGET_ACCESSOR(getSdlRectX, setSdlRectX)
    REGISTER_GADGET_ACCESSOR(getSdlRectY, setSdlRectY)
    REGISTER_GADGET_ACCESSOR(getSdlRectW, setSdlRectW)
    REGISTER_GADGET_ACCESSOR(getSdlRectH, setSdlRectH)
    REGISTER_GADGET_WRITER(setPosFromActor)
    REGISTER_GADGET_WRITER(setMoveGadget)
    {0, 0}
};

DEFINE_GADGET_CLASS(gTexture)

gTexture::gTexture():
    iTexture(0),
    iPosFromActor(true),
    i_gMove(0)
{

}

gTexture::~gTexture()
{

}

int gTexture::update(lua_State* /* aLua */, float /* aDelta */)
{
    return 0;
}

void gTexture::draw(SDL_Renderer* aRenderer, float aDelta)
{
    if (iIsEnabled) {
        if (iPosFromActor && i_gMove->isEnabled()) {
            glm::vec2 pos = getActor()->getPos();
            const glm::vec2& dir = i_gMove->getDir();
            float speed =  i_gMove->getSpeed();
            pos += dir * speed * aDelta;
            iSdlRect.x = pos.x + 0.5f;
            iSdlRect.y = pos.y + 0.5f;
        }

        SDL_RenderCopy(aRenderer, iTexture, 0, &iSdlRect);
    }
}

void gTexture::begin()
{
    iTexture = eTextureMgr::getMe()->getTexture(iName);

    if ( iPosFromActor && (i_gMove == 0) )
        std::cout << "gTexture ERROR: "<< iActor->getScript() << "; gMove gadget not set" << std::endl;
}
