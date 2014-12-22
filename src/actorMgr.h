#ifndef ACTOR_MGR_H
#define ACTOR_MGR_H

#include <list>
#include <lua.hpp>
#include <string>

class eActor;
class eLuaState;

class eActorMgr
{
public:
    eActorMgr();
    ~eActorMgr();

    static eActorMgr* getMe() { return iMe; }

    void update(lua_State* aLua);
    void doScript(lua_State* aLua);
    eActor* add(const std::string& aScript);

private:
    eActorMgr(const eActorMgr& aOther);
    eActorMgr& operator=(const eActorMgr& aOther);

private:
    static eActorMgr* iMe;
    std::list<eActor*> iActors;
};

#endif // ACTOR_MGR_H
