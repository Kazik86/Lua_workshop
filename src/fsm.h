#ifndef FSM_H
#define FSM_H

#include <lua.hpp>
#include <string>

class eLuaState;

class eFsm
{
public:
    eFsm(eLuaState& aLua, const std::string& aScript);
    ~eFsm();

    void doScript();
    void update();

private:
    eFsm(const eFsm& aOther);
    eFsm& operator=(const eFsm& aOther);

private:
    eLuaState& iLua;
    std::string iScript;
    int iModule;
    int iEnv;
};

#endif // FSM_H
