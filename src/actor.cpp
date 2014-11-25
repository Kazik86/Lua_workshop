#include "actor.h"

eActor::eActor(eLuaState& aLuaState, const std::string& aScript):
    iFsm(aLuaState, aScript)
{
}

void eActor::doScript()
{
    iFsm.doScript();
}

void eActor::update()
{
    iFsm.update();
}

