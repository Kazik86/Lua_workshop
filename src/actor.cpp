#include "actor.h"

eActor::eActor(eLuaState& aLua, const std::string& aScript):
    iFsm(aLua, aScript)
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

