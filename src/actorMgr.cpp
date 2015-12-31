#include "actorMgr.h"

#include "game.h"

#include <stdexcept>

eActorMgr* eActorMgr::iMe = 0;

namespace
{
    int newActor(lua_State* aLua)
    {
	eActorMgr* me = eActorMgr::getMe();
	eActor& a = me->add(aLua, Script::getVal<const char*>(aLua, 1), Script::getVal<unsigned int>(aLua, 2));
        lua_rawgeti(aLua, LUA_REGISTRYINDEX, a.getMeRef());
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

eActor& eActorMgr::add(lua_State* aLua, const std::string& aScript, size_t aParentId)
{
    if (iActorsNum == EActorsCapacity)
        throw std::runtime_error("eActorMgr: too much actors");

    eActor* a = new((void*)(iActors + iActorsNum)) eActor(aScript, iActorsNum, aParentId);

    // forbid "Main.lua" to register itself as its own child
    if (iActorsNum)
        incChildNum(aParentId);

    iActorsNum += 1;

    try {
        a->doScript(aLua);
    } catch (const std::exception& aErr) {

        destroyActor(a);
        throw;
    }

    return *a;
}

void eActorMgr::incChildNum(size_t aId)
{
    for (eActor* a = &iActors[aId]; a->getId() != a->getParentId(); a = &iActors[a->getParentId()])
        a->iChildNum += 1;

    iActors[0].iChildNum += 1;
}

void eActorMgr::decChildNum(size_t aId)
{
    for (eActor* a = &iActors[aId]; a->getId() != a->getParentId(); a = &iActors[a->getParentId()])
        a->iChildNum -= 1;

    iActors[0].iChildNum -= 1;
}

void eActorMgr::destroyChildren(const eActor* aActor)
{
    size_t n = aActor->getChildNum();
    size_t idx = aActor->getId();

    for (size_t i = n; i > 0; --i) {
        eActor& a = iActors[idx + i];
        decChildNum(a.getParentId());
        a.~eActor();
    }

    iActorsNum -= n;
}

void eActorMgr::destroyActor(const eActor* aActor)
{
    destroyChildren(aActor);
    decChildNum(aActor->getParentId());
    aActor->~eActor();
    --iActorsNum;
}
