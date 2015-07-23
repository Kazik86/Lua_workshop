#include "actorMgr.h"

#include <stdexcept>

eActorMgr* eActorMgr::iMe = 0;

namespace
{
    int newActor(lua_State* aLua)
    {
	eActorMgr* me = eActorMgr::getMe();
	eActor* a = me->add(aLua, Script::getVal<const char*>(aLua, 1));
	Script::pushVal<void*>(aLua, a);
	return 1;
    }
}

DEFINE_USERDATA_API(eActorMgr)
{
    {"add", ::newActor},
    {0, 0}
};

DEFINE_USERDATA_CLASS(eActorMgr)

eActorMgr::eActorMgr():
    iMainActor("scripts/Main.lua")
{
    if (iMe)
	throw std::runtime_error("eActorMgr: multiple instances not allowed.");

    iMe = this;
}

eActorMgr::~eActorMgr()
{
    for (eActor* a : iActors)
	delete a;

    iActors.clear();
    iMe = 0;
}

void eActorMgr::update(lua_State* aLua, float aDelta)
{
    iMainActor.update(aLua, aDelta);

    for (eActor* a : iActors)
	a->update(aLua, aDelta);
}

eActor* eActorMgr::add(lua_State* aLua, const std::string& aScript)
{
    eActor* a = new eActor(aScript);
    iActors.push_back(a);
    a->doScript(aLua);
    return a;
}

void eActorMgr::doMainScript(lua_State* aLua)
{
    iMainActor.doScript(aLua);
}
