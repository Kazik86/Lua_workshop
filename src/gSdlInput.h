#ifndef G_SDL_INPUT_H
#define G_SDL_INPUT_H

#include "gadget.h"

#include <SDL2/SDL.h>

class gSdlInput: public eGadget
{
    DECLARE_GADGET_CLASS()

public:
    gSdlInput();

    int update(lua_State* aLua, float aDelta);
    void begin(lua_State* aLua);

    // events
    DECLARE_GADGET_EVENT(keyboard)
    DECLARE_GADGET_EVENT(mouseButton)
    DECLARE_GADGET_EVENT(mouseMove)
    DECLARE_GADGET_EVENT(mouseWheel)

private:
    eLuaTable iEventData;
    SDL_Event iEvent;

    bool iMouseWheel;
    bool iMouseWheelSetInCurrentFrame;

    bool iMouseMove;
    bool iMouseMoveSetInCurrentFrame;
};

#endif // G_SDL_INPUT_H
