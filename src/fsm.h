#ifndef FSM_H
#define FSM_H

#include <lua.hpp>
#include <string>

class eLuaState;

class eFsmState
{
public:
    eFsmState();

private:
    eFsmState(const eFsmState& aOther);
    eFsmState& operator=(const eFsmState& aOther);
};

class eFsm
{
public:
    eFsm(eLuaState& aLuaState, const std::string& aScript);
    ~eFsm();

    void doScript();
    void update();
    int getModuleRef() const { return iModule; }

private:
    eFsm(const eFsm& aOther);
    eFsm& operator=(const eFsm& aOther);

    void loadModule(const char* aPath);
    void deriveModule(const char* aPath);
    void setClass(const char* aPath);

private:
    lua_State* iLuaState;
    std::string iScript;
    int iEnv;
    int iModule;
};

#endif // FSM_H
