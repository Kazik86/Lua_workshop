#ifndef FSM_H
#define FSM_H

#include <lua.hpp>

class eActor;
class eLuaState;

class eFsm
{
public:
    eFsm(eActor& aActor);
    ~eFsm();

    void update(lua_State* aLua);
    void shift(lua_State* aLua);

private:
    eFsm(const eFsm& aOther);
    eFsm& operator=(const eFsm& aOther);

    void setState(lua_State* aLua);

    void setFlag(unsigned int aFlag) { iFlags |= aFlag; }
    void unsetFlag(unsigned int aFlag) { iFlags &= ~aFlag; }
    bool checkFlag(unsigned int aFlag) const { return iFlags & aFlag; }

    void saveStage(lua_State* aLua, const char* aName, /* in/out */ int& aRef);
    void callFun(lua_State* aLua);
    void enter(lua_State* aLua);
    void leave(lua_State* aLua);

    void replaceEnv(lua_State* aLua);

private:
    eActor& iActor;
    unsigned int iFlags;
    int iStateRef;
    int iEnterRef;
    int iUpdateRef;
    int iLeaveRef;
};

#endif // FSM_H
