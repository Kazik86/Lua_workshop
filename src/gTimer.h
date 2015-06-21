#ifndef G_TIMER_H
#define G_TIMER_H

#include "gadget.h"

class gTimer: public eGadget
{
    DECLARE_GADGET_CLASS()

public:
    gTimer();

    int update(lua_State* aLua, float /* aDelta */);

    // props
    DECLARE_GADGET_ACCESSOR(getInterval, setInterval)

    // methods
    DECLARE_GADGET_METHOD(isElapsed)
    DECLARE_GADGET_METHOD(start)

    // events
    DECLARE_GADGET_EVENT(onElapsed)

private:
    bool isElapsed(lua_State* aLua);
    void start(lua_State* aLua);

private:
    unsigned int iStart;
    unsigned int iInterval;
};

#endif // G_TIMER_H
