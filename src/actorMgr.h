#ifndef ACTOR_MGR_H
#define ACTOR_MGR_H

#include <string>

class eActor;
class eLuaState;

namespace ActorMgr
{
    void init();
    void cleanup();
    void update();
    eActor& add(eLuaState& aLuaState, const std::string& aScript);
    void doScript();
}

#endif // ACTOR_MGR_H
