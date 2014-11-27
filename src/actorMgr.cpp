#include "actorMgr.h"

#include "actor.h"
#include <list>

namespace
{
    std::list<eActor*> iActors;
}

void ActorMgr::init()
{

}

void ActorMgr::cleanup()
{
    for (eActor* elm : iActors)
	delete elm;

    iActors.clear();
}

void ActorMgr::update()
{

}

eActor& ActorMgr::add(eLuaState& aLua, const std::string& aScript)
{
    iActors.push_front(new eActor(aLua, aScript));
    return *iActors.front();
}

void ActorMgr::doScript()
{
    for (eActor* elm : iActors)
	elm->doScript();
}
