#ifndef G_TEXTURE_H
#define G_TEXTURE_H

#include "gadget.h"
#include "renderer.h"

class gMove;
class gRotate;
class gTransform;

class gTexture: public eGadget, public eRenderable
{
    DECLARE_GADGET_CLASS()

public:
    gTexture();
    ~gTexture();

    void begin(lua_State* /* aLua */);
    int  update(lua_State* aLua, float aDelta);
    void draw(SDL_Renderer* aRenderer, float aDelta);

    // props
    DECLARE_GADGET_ACCESSOR(getName, setName)
    DECLARE_GADGET_ACCESSOR(getSdlRectX, setSdlRectX)
    DECLARE_GADGET_ACCESSOR(getSdlRectY, setSdlRectY)
    DECLARE_GADGET_ACCESSOR(getSdlRectW, setSdlRectW)
    DECLARE_GADGET_ACCESSOR(getSdlRectH, setSdlRectH)
    DECLARE_GADGET_WRITER(setPosFromActor)
    DECLARE_GADGET_WRITER(setMoveGadget)
    DECLARE_GADGET_WRITER(setRotFromActor)
    DECLARE_GADGET_WRITER(setRotateGadget)
    DECLARE_GADGET_WRITER(setZOrder)

    DECLARE_GADGET_METHOD(fadeIn)
    DECLARE_GADGET_METHOD(fadeOut)
    DECLARE_GADGET_METHOD(fadeOff)
    DECLARE_GADGET_METHOD(isFadeCompleted)
    DECLARE_GADGET_WRITER(setFadeDuration)
    DECLARE_GADGET_WRITER(setAlpha)

private:
    void fadeIn(lua_State* aLua);
    void fadeOut(lua_State* aLua);
    void fadeOff(lua_State* aLua);
    bool isFadeCompleted(lua_State* aLua);

private:
    SDL_Texture* iTexture;
    std::string iName;
    SDL_Rect iSdlRect;
    bool iPosFromActor;
    bool iRotFromActor;
    gMove* i_gMove;
    gRotate* i_gRotate;

    enum TFade { ENone = 0, EIn, EOut};
    TFade iFade;
    float iFadeDuration;
    float iFadeDelta;
    unsigned int iAlpha;

    gTransform* iTransform;
};

#endif // G_TEXTURE_H
