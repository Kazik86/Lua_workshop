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
    struct sFixture
    {
	~sFixture() { LuaModuleMgr::clear(); }
	eGame iGame;
    };

    // [-0, +0]
    void callLuaFun(lua_State* aLua, int aModuleRef, const char* aName, int aArgsNum, int aResultsNum)
    {
	lua_rawgeti(aLua, LUA_REGISTRYINDEX, aModuleRef);
	lua_getfield(aLua, -1, aName);
	lua_pcall(aLua, aArgsNum, aResultsNum, 0);

	CHECK(lua_toboolean(aLua, -1));

	lua_pop(aLua, 1 + aResultsNum);
    }
}

TEST_FIXTURE(sFixture, NonExistentScript)
{
    CHECK_THROW(LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/nonexistent.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, SameScript)
{
    int ref1 = LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/C.lua").iRef;
    int ref2 = LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/C.lua").iRef;
    
    CHECK(ref1 == ref2);
}

TEST_FIXTURE(sFixture, InvalidClassField1)
{
    CHECK_THROW(LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/invalidClass1.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, InvalidClassField2)
{
    CHECK_THROW(LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/invalidClass2.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, InvalidClassField3)
{
    CHECK_THROW(LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/invalidClass3.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassNameSavedInModule)
{
    const sModule& m = LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/A.lua");
    CHECK(m.iClass == "A");
}

TEST_FIXTURE(sFixture, Metatable)
{
    lua_State* lua = iGame.getLua()->getRaw();
    int ref = LuaModuleMgr::load(lua, "tests/scripts/C.lua").iRef;
    callLuaFun(lua, ref, "testMetatable", 0, 1);
}

TEST_FIXTURE(sFixture, Inheritance)
{
    lua_State* lua = iGame.getLua()->getRaw();
    int ref = LuaModuleMgr::load(lua, "tests/scripts/C.lua").iRef;
    callLuaFun(lua, ref, "testInheritance", 0, 1);
}

TEST_FIXTURE(sFixture, ClassUniqueness1)
{
    CHECK_THROW(LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/classUniqueness1/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassUniqueness2)
{
    CHECK_THROW(LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/classUniqueness2/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassUniqueness3)
{
    CHECK_THROW(LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/classUniqueness3/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, CircularInheritance1)
{
    CHECK_THROW(LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/circularInheritance1/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, CircularInheritance2)
{
    CHECK_THROW(LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/circularInheritance2/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, CircularInheritance3)
{
    CHECK_THROW(LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/circularInheritance3/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassNameUsedAsIdentifier)
{
    CHECK_THROW(LuaModuleMgr::load(iGame.getLua()->getRaw(), "tests/scripts/classNameUsedAsIdentifier.lua"), std::runtime_error);
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
    eActor a("tests/scripts/virtualFunctions/B.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.callLuaFunc(lua, "test");
    CHECK(true);
}

TEST_FIXTURE(sFixture, MeTableInheritance)
{
    eActor a("tests/scripts/meTableInheritance/C.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.callOnInit(lua);
    a.callLuaFunc(lua, "test1");
    a.callLuaFunc(lua, "test2");
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
    a.update(lua);
    a.update(lua);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, GadgetAndState)
{
    eActor a("tests/scripts/gadgetAndState.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);
    a.update(lua);
    a.update(lua);
    a.callLuaFunc(lua, "test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, ActorCreatedFromScript)
{
    CHECK(eActorMgr::getMe()->getActorsNum() == 0);

    eActor a("tests/scripts/actorCreatedFromScript.lua");
    lua_State* lua = iGame.getLua()->getRaw();
    a.doScript(lua);

    CHECK(eActorMgr::getMe()->getActorsNum() == 1);
}
