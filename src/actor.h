#ifndef ACTOR_H
#define ACTOR_H

#include "fsm.h"

#include <list>
#include <string>

class eLuaState;
struct sModule;

struct sActorSharedInternal
{
    sActorSharedInternal(const char* aName, void* aAddress):
	iName(aName),
	iAddress(aAddress) {}

    const char* iName;
    void* iAddress;
};

class eActor
{
    DECLARE_USERDATA_SUPPORT()

public:
    eActor(eLuaState& aLua, const std::string& aScript);
    ~eActor();

    void doScript();
    void update();
    void callOnInit();
    void callOnRestart();
    void callLuaFunc(const char* aFunctionName);
    const std::string& getScript() const { return iScript; }

private:
    eActor(const eActor& aOther);
    eActor& operator=(const eActor& aOther);

    void createMeTables();
    void callLuaFuncWithEnv(int aModuleRef, int aMeRef, const char* aFunctionName);
    void callLuaFuncThroughInheritanceHierarchyBackward(const char* aFunctionName);
    void shareInternalsWithScript();

private:
    eLuaState& iLua;
    eFsm iFsm;
    std::list<int> iMeRef;
    std::string iScript;
    const sModule* iModule;
};

#endif // ACTOR_H
