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

    // wrapper provides ability to customize _specific_ members from sGameProps
    const struct sGamePropsWrapper
    {
        sGamePropsWrapper()
        {
            iGameProps.iMainActorScript = "tests/Main.lua";
        }

        sGameProps iGameProps;

    } KGamePropsWrapper;

    struct sFixture
    {
        sFixture():
            iGame(KGamePropsWrapper.iGameProps) {}

	eGame iGame;
    };

    // [-0, +0]
    void callLuaFun(lua_State* aLua, int aModuleRef, const char* aName, int aArgsNum, int aResultsNum)
    {
	lua_rawgeti(aLua, LUA_REGISTRYINDEX, aModuleRef);
	lua_getfield(aLua, -1, aName);
	if (eLuaState::pcall(aLua, aArgsNum, aResultsNum) != LUA_OK)
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
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a1 = am->add(lua, "tests/scripts/meTable.lua", 0);
    eActor& a2 = am->add(lua, "tests/scripts/meTable.lua", 0);

    a1.callLuaFunc(lua, "setAttribs1");
    a2.callLuaFunc(lua, "setAttribs1");

    a1.callLuaFunc(lua, "checkAttribs1");
    a2.callLuaFunc(lua, "checkAttribs1");

    a1.callLuaFunc(lua, "setAttribs2");
    a1.callLuaFunc(lua, "checkAttribs2");
    a2.callLuaFunc(lua, "checkAttribs1");

    CHECK(true);
}

TEST_FIXTURE(sFixture, MeEnv)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/meEnv.lua", 0);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, VirtualFunctions)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/virtualFunctions/A.lua", 0);
    eActor& b = am->add(lua, "tests/scripts/virtualFunctions/B.lua", 0);

    b.callLuaFunc(lua, "test");
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, MeTableInheritance)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/meTableInheritance/C.lua", 0);

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
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/scriptSupport.lua", 0);
    eActor& b = am->add(lua, "tests/scripts/scriptSupport.lua", 0);

    a.callLuaFunc(lua, "setActorFoo");
    b.callLuaFunc(lua, "setActorBar");

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, GadgetTest)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/gadgetTest.lua", 0);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, StateShift)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/stateShift.lua", 0);

    a.update(lua, KDelta);
    a.update(lua, KDelta);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, GadgetAndState)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/gadgetAndState.lua", 0);
    
    a.update(lua, KDelta);
    a.update(lua, KDelta);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ActorCreatedFromScript)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    auto num = am->getActorsNum();
    am->add(lua, "tests/scripts/actorCreatedFromScript.lua", 0);

    CHECK(am->getActorsNum() == num + 2);
}

TEST_FIXTURE(sFixture, VirtualFunctionInsideState)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/virtualFunctionInsideState/a.lua", 0);
    eActor& b = am->add(lua, "tests/scripts/virtualFunctionInsideState/b.lua", 0);

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
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& b = am->add(lua, "tests/scripts/extendingState/b.lua", 0);
    b.update(lua, KDelta);
    b.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, RedefineStateBeingExtended)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& b = am->add(lua, "tests/scripts/extendingState/redefineStateBeingExtended/b.lua", 0);
    b.update(lua, KDelta);
    b.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, DontSearchInitInBase)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/dontSearchInitInBase/b.lua", 0);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, AutoEnableDisableGadgets1)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/autoEnableDisableGadgets/a.lua", 0);
    a.update(lua, KDelta);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, AutoEnableDisableGadgets2)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& b = am->add(lua, "tests/scripts/autoEnableDisableGadgets/b.lua", 0);
    b.update(lua, KDelta);
    b.update(lua, KDelta);

    b.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, GadgetStatus)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/gadgetStatus.lua", 0);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, GadgetEvent)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/gadgetEvent/a.lua", 0);

    a.update(lua, KDelta);
    a.update(lua, KDelta);
    a.update(lua, KDelta);
    a.update(lua, KDelta);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, BreakUpdateWhenShiftInExtended)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/breakUpdateWhenShiftInExtended/b.lua", 0);

    a.update(lua, KDelta);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, StateShiftInEventHandlerBreaksGadgetsAndStateUpdate)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/stateShiftInEventHandlerBreaksGadgetsAndStateUpdate.lua", 0);

    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/parentState/parentState.lua", 0);

    a.update(lua, KDelta);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState_redefineParent)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/parentState/redefineParent/b.lua", 0);

    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState_redefineChild)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/parentState/redefineChild/b.lua", 0);

    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState_extendParent)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/parentState/extendParent/b.lua", 0);

    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState_extendChild)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/parentState/extendChild/b.lua", 0);

    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState_redefineParentAndChild)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/parentState/redefineParentAndChild/b.lua", 0);

    a.update(lua, KDelta);
    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ParentState_extendParentAndChild)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/parentState/extendParentAndChild/b.lua", 0);

    a.update(lua, KDelta);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ScriptNamespaceInGlobalEnv)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/scriptNamespaceInGlobalEnv.lua", 0);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ActorsApi)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/actorsApi/B.lua", 0);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ActorsApi_virtualCall)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/actorsApi/virtualCall/main.lua", 0);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ActorsApi_replacingEnvWasABadIdea)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/actorsApi/replacingEnvWasABadIdea/A.lua", 0);

    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, DisableActor)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/disableActor/parent.lua", 0);

    a.update(lua, KDelta);
    a.update(lua, KDelta);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, Signals)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/signals/emit/sender.lua", 0);

    a.update(lua, KDelta);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, SignalsAndGC)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/signals/gc/main.lua", 0);

    a.update(lua, KDelta);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, SignalParams)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a = am->add(lua, "tests/scripts/signals/params/sender.lua", 0);

    a.update(lua, KDelta);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ActorHasChildrenInCpp)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();

    eActor& a0 = am->getActor(0);
    eActor& a1 = am->getActor(1);
    eActor& a2 = am->getActor(2);

    CHECK(am->getActorsNum() == 1); // "Main"
    CHECK(a0.getChildNum() == 0);

    am->add(lua, "tests/scripts/actorHasChildrenInCpp/parent.lua", 0);

    CHECK(a1.getId() == 1);
    CHECK(a1.getChildNum() == 3);
    CHECK(a1.getParentId() == 0);
    CHECK(a2.getParentId() == 1);
    CHECK(a2.getChildNum() == 1);
    CHECK(a0.getChildNum() == 4);
    CHECK(am->getActorsNum() == 5);

    am->destroyChildren(&a1);
    CHECK(a1.getChildNum() == 0);
    CHECK(a0.getChildNum() == 1);
    CHECK(am->getActorsNum() == 2);

    am->destroyActor(&a1);

    CHECK(a0.getChildNum() == 0);
    CHECK(am->getActorsNum() == 1);
}

TEST_FIXTURE(sFixture, SendParamsToInit)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();
    am->add(lua, "tests/scripts/sendParamsToInit/parent.lua", 0);
    CHECK(true);
}

TEST_FIXTURE(sFixture, DestroyActor)
{
    eActorMgr* am = eActorMgr::getMe();
    lua_State* lua = iGame.getLua()->getRaw();
    am->add(lua, "tests/scripts/destroyActor/parent.lua", 0);
    CHECK(true);
}
