#include "actorMgr.h"

#include "actor.h"

#include <stdexcept>

eActorMgr* eActorMgr::iMe = 0;

eActorMgr::eActorMgr()
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

void eActorMgr::update()
{
    for (eActor* a : iActors)
	a->update();
}

eActor* eActorMgr::add(eLuaState* aLua, const std::string& aScript)
{
    iActors.push_front(new eActor(aLua, aScript));
    return iActors.front();
}

void eActorMgr::doScript()
{
    for (eActor* a : iActors)
	a->doScript();
}
