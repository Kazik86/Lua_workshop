#include "gTexture.h"

#include "gRotate.h"
#include "gMove.h"
#include "gTransform.h"
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
DEFINE_GADGET_WRITER(gTexture, setRotFromActor, iRotFromActor)
DEFINE_GADGET_WRITER(gTexture, setRotateGadget, i_gRotate)

DEFINE_GADGET_METHOD_0(gTexture, fadeIn)
DEFINE_GADGET_METHOD_0(gTexture, fadeOut)
DEFINE_GADGET_METHOD_0(gTexture, fadeOff)
DEFINE_GADGET_METHOD_1(gTexture, isFadeCompleted)
DEFINE_GADGET_WRITER(gTexture, setFadeDuration, iFadeDuration)
DEFINE_GADGET_WRITER(gTexture, setAlpha, iAlpha)

DEFINE_GADGET_API(gTexture)
{
    REGISTER_GADGET_ACCESSOR(getName, setName)
    REGISTER_GADGET_ACCESSOR(getSdlRectX, setSdlRectX)
    REGISTER_GADGET_ACCESSOR(getSdlRectY, setSdlRectY)
    REGISTER_GADGET_ACCESSOR(getSdlRectW, setSdlRectW)
    REGISTER_GADGET_ACCESSOR(getSdlRectH, setSdlRectH)
    REGISTER_GADGET_WRITER(setPosFromActor)
    REGISTER_GADGET_WRITER(setMoveGadget)
    REGISTER_GADGET_WRITER(setRotFromActor)
    REGISTER_GADGET_WRITER(setRotateGadget)

    REGISTER_GADGET_METHOD(fadeIn)
    REGISTER_GADGET_METHOD(fadeOut)
    REGISTER_GADGET_METHOD(fadeOff)
    REGISTER_GADGET_METHOD(isFadeCompleted)
    REGISTER_GADGET_WRITER(setFadeDuration)
    REGISTER_GADGET_WRITER(setAlpha)
    {0, 0}
};

DEFINE_GADGET_CLASS(gTexture)

gTexture::gTexture():
    iTexture(0),
    iSdlRect(),
    iPosFromActor(true),
    iRotFromActor(true),
    i_gMove(0),
    i_gRotate(0),
    iFade(ENone),
    iFadeDuration(1),
    iFadeDelta(0),
    iAlpha(1),
    iTransform(0)
{
    iTransform = static_cast<gTransform*>(getActor()->findGadgetByClass("gTransform"));
    if (! iTransform) throw std::runtime_error(getActor()->getScript() + ", gTexture: 'gTransform' not found.");
}

gTexture::~gTexture()
{

}

void gTexture::begin(lua_State* /* aLua */)
{
    iTexture = eTextureMgr::getMe()->getTexture(iName);

    if (iSdlRect.w == 0 || iSdlRect.h == 0) {
        int w, h;
        SDL_QueryTexture(iTexture, 0, 0, &w, &h);
        if (iSdlRect.w == 0) iSdlRect.w = w;
        if (iSdlRect.h == 0) iSdlRect.h = h;
    }

    if ( iPosFromActor && (i_gMove == 0) )
        std::cout << "gTexture ERROR: "<< iActor->getScript() << "; gMove gadget not set" << std::endl;

    if ( iRotFromActor && (i_gRotate == 0) )
        std::cout << "gTexture ERROR: "<< iActor->getScript() << "; gRotate gadget not set" << std::endl;
}

int gTexture::update(lua_State* /* aLua */, float aDelta)
{
    if (iFade) {
        iFadeDelta += aDelta;
        iFadeDelta = glm::clamp(iFadeDelta, 0.f, iFadeDuration);
        iAlpha = iFade == EOut ? (1.f - iFadeDelta/iFadeDuration) * 255 : /* fade in */ iFadeDelta/iFadeDuration * 255;
        SDL_SetTextureAlphaMod(iTexture, iAlpha);
    }

    addToRenderingQueue();

    return 0;
}

void gTexture::draw(SDL_Renderer* aRenderer, float aDelta)
{
    if (iIsEnabled) {
        if (iPosFromActor && i_gMove && i_gMove->isEnabled()) {
            glm::vec2 pos = iTransform->getPos();
            const glm::vec2& dir = i_gMove->getDir();
            float speed =  i_gMove->getSpeed();
            pos += dir * speed * aDelta;
            iSdlRect.x = pos.x + 0.5f;
            iSdlRect.y = pos.y + 0.5f;
        }

        double angle = iTransform->getRotate();
        if (iRotFromActor && i_gRotate && i_gRotate->isEnabled()) {
            int dir = i_gRotate->getDir();
            float speed =  i_gRotate->getOmega();
            angle += dir * speed * aDelta;
        }

        SDL_RenderCopyEx(aRenderer, iTexture, 0, &iSdlRect, angle, NULL, SDL_FLIP_NONE);
    }
}

void gTexture::fadeIn(lua_State*)
{
    iFade = EIn;
    SDL_SetTextureBlendMode(iTexture, SDL_BLENDMODE_BLEND);
    iFadeDelta = 0;
}

void gTexture::fadeOut(lua_State*)
{
    iFade = EOut;
    SDL_SetTextureBlendMode(iTexture, SDL_BLENDMODE_BLEND);
    iFadeDelta = 0;
}

void gTexture::fadeOff(lua_State*)
{
    iFade = ENone;
    SDL_SetTextureBlendMode(iTexture, SDL_BLENDMODE_NONE);
    SDL_SetTextureAlphaMod(iTexture, 255);
}

bool gTexture::isFadeCompleted(lua_State*)
{
    if (iFade == EIn) return iAlpha == 255;
    if (iFade == EOut) return iAlpha == 0;

    return true;
}
