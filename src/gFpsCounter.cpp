#include "gFpsCounter.h"

// methods
DEFINE_GADGET_METHOD_0(gFpsCounter, setZOrder)

DEFINE_GADGET_API(gFpsCounter)
{
    // methods
    REGISTER_GADGET_METHOD(setZOrder)
    {0, 0}
};

DEFINE_GADGET_CLASS(gFpsCounter)

gFpsCounter::gFpsCounter():
    iText(0, 0, eFontMgr::ETtfMedium),
    iFrameCntr(0),
    iDelta(0)
{

}

int gFpsCounter::update(lua_State* /* aLua */, float aDelta)
{
    iDelta += aDelta;
    static float fps = 0;

    if (iDelta > 1) {
        fps = float(iFrameCntr)/iDelta;
        iFrameCntr = 0;
        iDelta = 0;
    }

    iText.format("fps: %.2f", fps);

    iText.addToRenderingQueue();
    addToRenderingQueue();

    return 0;
}

void gFpsCounter::draw(SDL_Renderer* /* aRenderer */, float /* aDelta */)
{
    ++iFrameCntr;
}
