#ifndef ACTOR_H
#define ACTOR_H

#include "fsm.h"
#include "luaModule.h"
#include "userdata.h"

#include <list>
#include <string>

class eGadget;
class eLuaState;

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
    DECLARE_USERDATA_CLASS()

    friend class eActorMgr;

public:
    eActor(const std::string& aScript, size_t aId, size_t aParentId);
    ~eActor();

    size_t getId() const { return iId; }
    size_t getChildNum() const { return iChildNum; }
    size_t getParentId() const { return iParentId; }

    void doScript(lua_State* aLua);
    void update(lua_State* aLua, float aDelta);
    void callInit(lua_State* aLua);
    void callRestart(lua_State* aLua);
    void callLuaFunc(lua_State* aLua, const char* aFunctionName);
    const std::string& getScript() const { return iScript; }
    std::list<eGadget*>::size_type getGadgetsNum() const { return iGadgets.size(); }
    void shift(lua_State* aLua) { iFsm.shift(aLua); }
    int getMeRef() const { return iMeRef.front(); }
    int getModuleRef() const { return iModule->iRef; }
    void addGadget(eGadget* aGadget);
    void enable() { iEnabled = true; }
    void disable() { iEnabled = false; }
    bool isEnabled() const { return iEnabled; }
    eGadget* findGadgetByClass(const char* aClass) const;

private:
    eActor(const eActor& aOther);
    eActor& operator=(const eActor& aOther);

    typedef bool (eActor::*TLuaCaller)(lua_State*, const sModule*, int, const char*);

    void createMeTables(lua_State* aLua);

    bool callLuaFuncShallow(lua_State* aLua, const sModule* aModule, int aMeRef, const char* aFunctionName);
    bool callLuaFuncShallow_2(lua_State* aLua, const sModule* aModule, int aMeRef, const char* aFunctionName);
    void callLuaFuncDeep(lua_State* aLua, const sModule* aModule, int aMeRef, const char* aFunctionName);
    void callLuaFuncThroughInheritanceHierarchyBackward(lua_State* aLua, const char* aFunctionName, TLuaCaller aFun);

    void shareInternalsWithScript(lua_State* aLua, int aRef);
    void beginGadget(lua_State* aLua);
    void shiftToState(lua_State* aLua, const char* aName);
    void shiftToEntryState(lua_State* aLua);
    void reenterState(lua_State* aLua);

private:
    size_t iId;
    size_t iChildNum;
    size_t iParentId;

    eFsm iFsm;
    std::list<int> iMeRef;
    std::string iScript;
    const sModule* iModule;
    std::list<eGadget*> iGadgets;
    bool iEnabled;
};

#endif // ACTOR_H
