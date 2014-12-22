#ifndef ACTOR_MGR_H
#define ACTOR_MGR_H

#include "actor.h"
#include "userdata.h"

#include <list>
#include <lua.hpp>
#include <string>

class eActor;
class eLuaState;

class eActorMgr
{
    DECLARE_USERDATA_CLASS()

public:
    eActorMgr();
    ~eActorMgr();

    static eActorMgr* getMe() { return iMe; }

    void update(lua_State* aLua);
    void doScript(lua_State* aLua);
    eActor* add(const std::string& aScript);
    std::list<eActor*>::size_type getActorsNum() const { return iActors.size(); }

private:
    eActorMgr(const eActorMgr& aOther);
    eActorMgr& operator=(const eActorMgr& aOther);

private:
    static eActorMgr* iMe;
    eActor iMainActor;
    std::list<eActor*> iActors;
};

#endif // ACTOR_MGR_H
