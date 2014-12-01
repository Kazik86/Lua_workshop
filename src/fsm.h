#ifndef FSM_H
#define FSM_H

#include <list>
#include <lua.hpp>
#include <string>

class eLuaState;
struct sModule;

class eFsm
{
public:
    eFsm(eLuaState& aLua, const std::string& aScript);
    ~eFsm();

    void doScript();
    void update();
    void callOnInit();
    void callOnRestart();
    void callLuaFunc(const char* aFunctionName);

private:
    eFsm(const eFsm& aOther);
    eFsm& operator=(const eFsm& aOther);

    void createMeTables();
    void callLuaFuncWithEnv(int aModuleRef, int aMeRef, const char* aFunctionName);
    void callLuaFuncThroughInheritanceHierarchyBackward(const char* aFunctionName);

private:
    eLuaState& iLua;
    std::string iScript;
    const sModule* iModule;
    std::list<int> iMeRef;
};

#endif // FSM_H
