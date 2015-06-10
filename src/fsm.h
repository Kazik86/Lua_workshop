#ifndef FSM_H
#define FSM_H

#include <lua.hpp>
#include <string>

class eActor;
class eLuaState;

class eFsm
{
public:
    eFsm(eActor& aActor);
    ~eFsm();

    void update(lua_State* aLua);
    void shift(lua_State* aLua);
    int getStateRef() const { return iStateRef; }
    const std::string& getFullName() const { return iName; }

private:
    eFsm(const eFsm& aOther);
    eFsm& operator=(const eFsm& aOther);

    void setFlag(unsigned int aFlag) { iFlags |= aFlag; }
    void unsetFlag(unsigned int aFlag) { iFlags &= ~aFlag; }
    bool checkFlag(unsigned int aFlag) const { return iFlags & aFlag; }

    void saveStage(lua_State* aLua, const char* aName, /* in/out */ int& aRef);

    void setName(lua_State* aLua);

private:
    eActor& iActor;
    unsigned int iFlags;
    int iStateRef;
    int iUpdateRef;
    std::string iName;
};

#endif // FSM_H
