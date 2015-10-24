#ifndef ACTOR_H
#define ACTOR_H

#include "fsm.h"
#include "luaModule.h"
#include "userdata.h"

#include <glm/glm.hpp>
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

public:
    eActor(const std::string& aScript);
    ~eActor();

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

    const glm::vec2& getPos() const { return iPos; }
    void setPos(const glm::vec2& aPos) { iPos = aPos; }

    double getRotate() const { return iRot; }
    void setRotate(double aRot) { iRot = aRot; }

private:
    eActor(const eActor& aOther);
    eActor& operator=(const eActor& aOther);

    void createMeTables(lua_State* aLua);
    void callLuaFuncShallow(lua_State* aLua, const sModule* aModule, int aMeRef, const char* aFunctionName, bool aThrow);
    void callLuaFuncDeep(lua_State* aLua, const sModule* aModule, int aMeRef, const char* aFunctionName);
    void callLuaFuncThroughInheritanceHierarchyBackward(lua_State* aLua, const char* aFunctionName);
    void shareInternalsWithScript(lua_State* aLua, int aRef);
    void beginGadget();
    void shiftToState(lua_State* aLua, const char* aName);
    void shiftToEntryState(lua_State* aLua);
    void reenterState(lua_State* aLua);

private:
    eFsm iFsm;
    std::list<int> iMeRef;
    std::string iScript;
    const sModule* iModule;
    std::list<eGadget*> iGadgets;
    glm::vec2 iPos;
    double iRot;
    bool iEnabled;
};

#endif // ACTOR_H
