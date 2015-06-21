#include "gTimer.h"

#include <SDL2/SDL.h>

// props
DEFINE_GADGET_ACCESSOR(gTimer, getInterval, setInterval, iInterval)

// methods
DEFINE_GADGET_METHOD_1(gTimer, isElapsed)
DEFINE_GADGET_METHOD_0(gTimer, start)

// events
DEFINE_GADGET_EVENT(gTimer, onElapsed)

DEFINE_GADGET_API(gTimer)
{
    // props
    REGISTER_GADGET_ACCESSOR(getInterval, setInterval)

    // methods
    REGISTER_GADGET_METHOD(isElapsed)
    REGISTER_GADGET_METHOD(start)

    // events
    REGISTER_GADGET_EVENT(onElapsed)

    {0, 0}
};

DEFINE_GADGET_CLASS(gTimer)

gTimer::gTimer():
    iStart(0),
    iInterval(0)
{

}

int gTimer::update(lua_State* aLua, float /* aDelta */)
{
    if (iInterval > 0) {
        if ((SDL_GetTicks() - iStart) > iInterval)
            return emit(aLua, onElapsed);
    }

    return 0;
}

bool gTimer::isElapsed(lua_State* aLua)
{
    return (SDL_GetTicks() - iStart) > lua_tounsigned(aLua, 2);
}

void gTimer::start(lua_State* /* aLua */)
{
    iStart = SDL_GetTicks();
}
