#include "actorMgr.h"

#include "game.h"

#include <stdexcept>

eActorMgr* eActorMgr::iMe = 0;

namespace
{
    int newActor(lua_State* aLua)
    {
	eActorMgr* me = eActorMgr::getMe();
	int meRef = me->add(aLua, Script::getVal<const char*>(aLua, 1));
        lua_rawgeti(aLua, LUA_REGISTRYINDEX, meRef);
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
    iActors(0),
    iActorsNum(0)
{
    if (iMe)
	throw std::runtime_error("eActorMgr: multiple instances not allowed.");

    iMe = this;
}

void eActorMgr::init()
{
    iActors = static_cast<eActor*>(::operator new(EActorsCapacity * sizeof(eActor)));
}

eActorMgr::~eActorMgr()
{
    for (size_t i = iActorsNum; i > 0; --i)
        iActors[i - 1].~eActor();

    ::operator delete((void*)iActors);

    iActorsNum = 0;
    iActors = 0;
    iMe = 0;
}

void eActorMgr::update(lua_State* aLua, float aDelta)
{
    for (size_t i = 0; i < iActorsNum; ++i)
	iActors[i].update(aLua, aDelta);
}

int eActorMgr::add(lua_State* aLua, const std::string& aScript)
{
    if (iActorsNum == EActorsCapacity)
        throw std::runtime_error("eActorMgr: too much actors");

    eActor* a = new((void*)(iActors + iActorsNum)) eActor(aScript);
    iActorsNum += 1;

    try {
        a->doScript(aLua);
    } catch (const std::exception& aErr) {
        iActorsNum -= 1;
        a->~eActor();
        throw;
    }

    return a->getMeRef();
}

