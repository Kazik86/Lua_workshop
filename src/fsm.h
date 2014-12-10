#ifndef FSM_H
#define FSM_H

#include "scripting.h"

class eLuaState;

class eFsm
{
public:
    eFsm(eLuaState& aLua);
    ~eFsm();

    void update();

private:
    eFsm(const eFsm& aOther);
    eFsm& operator=(const eFsm& aOther);

private:
    eLuaState& iLua;
};

#endif // FSM_H
