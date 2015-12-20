#ifndef ACTOR_MGR_H
#define ACTOR_MGR_H

#include "actor.h"
#include "userdata.h"

#include <lua.hpp>
#include <string>

class eActor;
class eLuaState;

class eActorMgr
{
    DECLARE_USERDATA_CLASS()

    enum { EActorsCapacity = 64 };

public:
    eActorMgr();
    ~eActorMgr();

    static eActorMgr* getMe() { return iMe; }

    void init();
    void update(lua_State* aLua, float aDelta);
    int add(lua_State* aLua, const std::string& aScript);
    unsigned int getActorsNum() const { return iActorsNum; }

private:
    eActorMgr(const eActorMgr& aOther);
    eActorMgr& operator=(const eActorMgr& aOther);

private:
    static eActorMgr* iMe;
    eActor* iActors;
    unsigned int iActorsNum;
};

#endif // ACTOR_MGR_H
