#ifndef ACTOR_MGR_H
#define ACTOR_MGR_H

#include <list>
#include <string>

class eActor;
class eLuaState;

class eActorMgr
{
public:
    eActorMgr();
    ~eActorMgr();

    static eActorMgr* getMe() { return iMe; }

    void update();
    void doScript();
    eActor* add(eLuaState* aLua, const std::string& aScript);

private:
    eActorMgr(const eActorMgr& aOther);
    eActorMgr& operator=(const eActorMgr& aOther);

private:
    static eActorMgr* iMe;
    std::list<eActor*> iActors;
};

#endif // ACTOR_MGR_H
