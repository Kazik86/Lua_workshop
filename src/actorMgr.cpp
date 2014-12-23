#include "actorMgr.h"

#include <stdexcept>

eActorMgr* eActorMgr::iMe = 0;

namespace
{
    int newActor(lua_State* aLua)
    {
	eActorMgr* me = eActorMgr::getMe();
	eActor* a = me->add(Script::getVal<const char*>(aLua, 1));
	Script::pushVal<void*>(aLua, a);
	return 1;
    }
}

DEFINE_USERDATA_API(eActorMgr)
{
    {"newActor", ::newActor},
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

eActor* eActorMgr::add(const std::string& aScript)
{
    iActors.push_front(new eActor(aScript));
    return iActors.front();
}

void eActorMgr::doScript(lua_State* aLua)
{
    iMainActor.doScript(aLua);

    for (eActor* a : iActors)
	a->doScript(aLua);
}
