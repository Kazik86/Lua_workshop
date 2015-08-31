#include <UnitTest++/UnitTest++.h>

#include "actor.h"
#include "actorMgr.h"
#include "gadget.h"
#include "game.h"
#include "luaModule.h"
#include "luaState.h"

#include <stdexcept>

namespace
{
    const float KDelta = float(30) / float(1000);

    struct sFixture
    {
	eGame iGame;
    };

    // [-0, +0]
    void callLuaFun(lua_State* aLua, int aModuleRef, const char* aName, int aArgsNum, int aResultsNum)
    {
	lua_rawgeti(aLua, LUA_REGISTRYINDEX, aModuleRef);
	lua_getfield(aLua, -1, aName);
	if (lua_pcall(aLua, aArgsNum, aResultsNum, 0) != LUA_OK)
	    throw std::runtime_error(lua_tostring(aLua, -1));

	CHECK(lua_toboolean(aLua, -1));

	lua_pop(aLua, 1 + aResultsNum);
    }
}

TEST_FIXTURE(sFixture, NonExistentScript)
{
    CHECK_THROW(eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/nonexistent.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, SameScript)
{
    int ref1 = eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/C.lua").iRef;
    int ref2 = eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/C.lua").iRef;
    
    CHECK(ref1 == ref2);
}

TEST_FIXTURE(sFixture, InvalidClassField1)
{
    CHECK_THROW(eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/invalidClass1.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, InvalidClassField2)
{
    CHECK_THROW(eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/invalidClass2.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, InvalidClassField3)
{
    CHECK_THROW(eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/invalidClass3.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassNameSavedInModule)
{
    const sModule& m = eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/A.lua");
    CHECK(m.iClass == "A");
}

TEST_FIXTURE(sFixture, Metatable)
{
    lua_State* lua = iGame.getLua()->getRaw();
    int ref = eLuaModuleMgr::getMe()->load(lua, "tests/scripts/C.lua").iRef;
    callLuaFun(lua, ref, "testMetatable", 0, 1);
}

TEST_FIXTURE(sFixture, Inheritance)
{
    lua_State* lua = iGame.getLua()->getRaw();
    int ref = eLuaModuleMgr::getMe()->load(lua, "tests/scripts/C.lua").iRef;
    callLuaFun(lua, ref, "testInheritance", 0, 1);
}

TEST_FIXTURE(sFixture, ClassUniqueness1)
{
    CHECK_THROW(eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/classUniqueness1/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassUniqueness2)
{
    CHECK_THROW(eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/classUniqueness2/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassUniqueness3)
{
    CHECK_THROW(eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/classUniqueness3/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, CircularInheritance1)
{
    CHECK_THROW(eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/circularInheritance1/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, CircularInheritance2)
{
    CHECK_THROW(eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/circularInheritance2/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, CircularInheritance3)
{
    CHECK_THROW(eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/circularInheritance3/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassNameUsedAsIdentifier)
{
    CHECK_THROW(eLuaModuleMgr::getMe()->load(iGame.getLua()->getRaw(), "tests/scripts/classNameUsedAsIdentifier.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, MeTable)
{
    eActor a1("tests/scripts/meTable.lua");
    eActor a2("tests/scripts/meTable.lua");

    lua_State* lua = iGame.getLua()->getRaw();

    a1.doScript(lua);
    a2.doScript(lua);

    a1.callLuaFunc(lua, "setAttribs1");
    a2.callLuaFunc(lua, "setAttribs1");

    a1.callLuaFunc(lua, "checkAttribs1");
    a2.callLuaFunc(lua, "checkAttribs1");

    a1.callLuaFunc(lua, "setAttribs2");
    a1.callLuaFunc(lua, "checkAttribs2");
    a2.callLuaFunc(lua, "checkAttribs1");

    CHECK(true);
}

TEST_FIXTURE(sFixture, VirtualFunctions)
{
    eActor a("tests/scripts/virtualFunctions/A.lua");
    eActor b("tests/scripts/virtualFunctions/B.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    b.doScript(lua);
    b.callLuaFunc(lua, "test");
    a.doScript(lua);
    a.callLuaFunc(lua, "test");
    CHECK(true);
}

TEST_FIXTURE(sFixture, MeTableInheritance)
{
    eActor a("tests/scripts/meTableInheritance/C.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.callLuaFunc(lua, "testInA_1");
    a.callLuaFunc(lua, "testInB_1");
    a.callLuaFunc(lua, "testInC_1");
    a.callLuaFunc(lua, "testInC_2");
    a.callLuaFunc(lua, "testInB_2");
    a.callLuaFunc(lua, "testInA_2");
    CHECK(true);
}

TEST_FIXTURE(sFixture, ScriptSupport)
{
    eActor a("tests/scripts/scriptSupport.lua");
    eActor b("tests/scripts/scriptSupport.lua");

    lua_State* lua = iGame.getLua()->getRaw();

    a.doScript(lua);
    b.doScript(lua);

    a.callLuaFunc(lua, "setActorFoo");
    b.callLuaFunc(lua, "setActorBar");

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, GadgetTest)
{
    eActor a("tests/scripts/gadgetTest.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, StateShift)
{
    eActor a("tests/scripts/stateShift.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua, KDelta);
    a.update(lua, KDelta);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, GadgetAndState)
{
    eActor a("tests/scripts/gadgetAndState.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua, KDelta);
    a.update(lua, KDelta);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ActorCreatedFromScript)
{
    auto num = eActorMgr::getMe()->getActorsNum();

    eActor a("tests/scripts/actorCreatedFromScript.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);

    CHECK(eActorMgr::getMe()->getActorsNum() == num + 1);
}

TEST_FIXTURE(sFixture, VirtualFunctionInsideState)
{
    eActor a("tests/scripts/virtualFunctionInsideState/a.lua");
    eActor b("tests/scripts/virtualFunctionInsideState/b.lua");

    lua_State* lua = iGame.getLua()->getRaw();

    a.doScript(lua);
    b.doScript(lua);

    b.update(lua, KDelta);
    b.update(lua, KDelta);
    a.update(lua, KDelta);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");
    b.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ExtendingState)
{
    eActor b("tests/scripts/extendingState/b.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    b.doScript(lua);
    b.update(lua, KDelta);
    b.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, RedefineStateBeingExtended)
{
    eActor b("tests/scripts/extendingState/redefineStateBeingExtended/b.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    b.doScript(lua);
    b.update(lua, KDelta);
    b.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, DontSearchInitInBase)
{
    eActor a("tests/scripts/dontSearchInitInBase/b.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, AutoEnableDisableGadgets1)
{
    eActor a("tests/scripts/autoEnableDisableGadgets/a.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua, KDelta);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, AutoEnableDisableGadgets2)
{
    eActor b("tests/scripts/autoEnableDisableGadgets/b.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    b.doScript(lua);
    b.update(lua, KDelta);
    b.update(lua, KDelta);

    b.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ReplaceEnv)
{
    eActor a("tests/scripts/replaceEnv/b.lua");

    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, GadgetStatus)
{
    eActor a("tests/scripts/gadgetStatus.lua");

    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, GadgetEvent)
{
    eActor a("tests/scripts/gadgetEvent/a.lua");

    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua, KDelta);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, BreakUpdateWhenShiftInExtended)
{
    eActor a("tests/scripts/breakUpdateWhenShiftInExtended/b.lua");

    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua, KDelta);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, StateShiftInEventHandlerBreaksGadgetsAndStateUpdate)
{
    eActor a("tests/scripts/stateShiftInEventHandlerBreaksGadgetsAndStateUpdate.lua");

    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState)
{
    eActor a("tests/scripts/parentState/parentState.lua");

    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua, KDelta);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState_redefineParent)
{
    eActor a("tests/scripts/parentState/redefineParent/b.lua");

    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState_redefineChild)
{
    eActor a("tests/scripts/parentState/redefineChild/b.lua");

    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState_extendParent)
{
    eActor a("tests/scripts/parentState/extendParent/b.lua");

    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState_extendChild)
{
    eActor a("tests/scripts/parentState/extendChild/b.lua");

    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState_redefineParentAndChild)
{
    eActor a("tests/scripts/parentState/redefineParentAndChild/b.lua");

    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua, KDelta);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState_extendParentAndChild)
{
    eActor a("tests/scripts/parentState/extendParentAndChild/b.lua");

    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ScriptNamespaceInGlobalEnv)
{
    eActor a("tests/scripts/scriptNamespaceInGlobalEnv.lua");

    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

