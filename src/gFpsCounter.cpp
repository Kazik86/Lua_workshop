#include "gFpsCounter.h"

DEFINE_GADGET_API(gFpsCounter)
{
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
    static int fps = 0;

    if (iDelta > 1) {
        fps = int( (float(iFrameCntr) / iDelta) + 0.5f );
        iFrameCntr = 0;
        iDelta -= 1;
    }

    iText.format("fps: %d", fps);

    return 0;
}

void gFpsCounter::draw(SDL_Renderer* /* aRenderer */, float /* aDelta */)
{
    ++iFrameCntr;
}
