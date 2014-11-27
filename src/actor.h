#ifndef ACTOR_H
#define ACTOR_H

#include "fsm.h"

class eLuaState;

class eActor
{
public:
    eActor(eLuaState& aLua, const std::string& aScript);

    void doScript();
    void update();

private:
    eActor(const eActor& aOther);
    eActor& operator=(const eActor& aOther);

private:
    eFsm iFsm;
};

#endif // ACTOR_H
