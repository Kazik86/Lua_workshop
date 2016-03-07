#ifndef G_FPS_COUNTER_H
#define G_FPS_COUNTER_H

#include "gadget.h"
#include "renderer.h"
#include "text.h"

class gFpsCounter: public eGadget, public eRenderable
{
    DECLARE_GADGET_CLASS()

public:
    gFpsCounter();

    int update(lua_State* /* aLua */, float /* aDelta */);
    void draw(SDL_Renderer* aRenderer, float aDelta);

    // methods
    DECLARE_GADGET_METHOD(setZOrder)

private:
    void setZOrder(lua_State* aLua) { iText.setZOrder(Script::getVal<int>(aLua, 2)); }

private:
    eText iText;
    int iFrameCntr;
    float iDelta;
};

#endif // G_FPS_COUNTER_H
