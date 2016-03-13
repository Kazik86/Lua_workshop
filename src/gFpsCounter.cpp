#include "gFpsCounter.h"

#include "renderer.h"

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
    iDelta(0)
{

}

void gFpsCounter::enable()
{
    eRenderer::getMe()->resetFrameCntr();
}

int gFpsCounter::update(lua_State* /* aLua */, float aDelta)
{
    iDelta += aDelta;
    static float fps = 0;

    if (iDelta > 1) {
        eRenderer* renderer = eRenderer::getMe();
        fps = float(renderer->getFrameCntr())/iDelta;
        renderer->resetFrameCntr();
        iDelta = 0;
    }

    iText.format("fps: %.2f", fps);
    iText.addToRenderingQueue();

    return 0;
}
