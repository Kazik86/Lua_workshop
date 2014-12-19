#include <UnitTest++/UnitTest++.h>

#include "actor.h"
#include "gadget.h"
#include "luaModule.h"
#include "luaState.h"

#include <stdexcept>

namespace
{
    struct sFixture
    {
	~sFixture() { LuaModuleMgr::clear(); }
	eLuaState iLua;
    };

    // [-0, +0]
    void callLuaFun(eLuaState& aLua, int aModuleRef, const char* aName, int aArgsNum, int aResultsNum)
    {
	lua_State* lua = aLua.getRaw();

	lua_rawgeti(lua, LUA_REGISTRYINDEX, aModuleRef);
	lua_getfield(lua, -1, aName);
	lua_pcall(lua, aArgsNum, aResultsNum, 0);

	CHECK(lua_toboolean(lua, -1));

	lua_pop(lua, 1 + aResultsNum);
    }
}

TEST_FIXTURE(sFixture, NonExistentScript)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests/scripts/nonexistent.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, SameScript)
{
    int ref1 = LuaModuleMgr::load(iLua, "tests/scripts/C.lua").iRef;
    int ref2 = LuaModuleMgr::load(iLua, "tests/scripts/C.lua").iRef;
    
    CHECK(ref1 == ref2);
}

TEST_FIXTURE(sFixture, InvalidClassField1)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests/scripts/invalidClass1.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, InvalidClassField2)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests/scripts/invalidClass2.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, InvalidClassField3)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests/scripts/invalidClass3.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassNameSavedInModule)
{
    const sModule& m = LuaModuleMgr::load(iLua, "tests/scripts/A.lua");
    CHECK(m.iClass == "A");
}

TEST_FIXTURE(sFixture, Metatable)
{
    int ref = LuaModuleMgr::load(iLua, "tests/scripts/C.lua").iRef;
    callLuaFun(iLua, ref, "testMetatable", 0, 1);
}

TEST_FIXTURE(sFixture, Inheritance)
{
    int ref = LuaModuleMgr::load(iLua, "tests/scripts/C.lua").iRef;
    callLuaFun(iLua, ref, "testInheritance", 0, 1);
}

TEST_FIXTURE(sFixture, ClassUniqueness1)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests/scripts/classUniqueness1/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassUniqueness2)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests/scripts/classUniqueness2/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassUniqueness3)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests/scripts/classUniqueness3/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, CircularInheritance1)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests/scripts/circularInheritance1/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, CircularInheritance2)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests/scripts/circularInheritance2/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, CircularInheritance3)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests/scripts/circularInheritance3/C.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassNameUsedAsIdentifier)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests/scripts/classNameUsedAsIdentifier.lua"), std::runtime_error);
}

TEST_FIXTURE(sFixture, MeTable)
{
    eActor a1(iLua, "tests/scripts/meTable.lua");
    eActor a2(iLua, "tests/scripts/meTable.lua");

    a1.doScript();
    a2.doScript();

    a1.callLuaFunc("setAttribs1");
    a2.callLuaFunc("setAttribs1");

    a1.callLuaFunc("checkAttribs1");
    a2.callLuaFunc("checkAttribs1");

    a1.callLuaFunc("setAttribs2");
    a1.callLuaFunc("checkAttribs2");
    a2.callLuaFunc("checkAttribs1");

    CHECK(true);
}

TEST_FIXTURE(sFixture, VirtualFunctions)
{
    eActor a(iLua, "tests/scripts/virtualFunctions/B.lua");
    a.doScript();
    a.callLuaFunc("test");
    CHECK(true);
}

TEST_FIXTURE(sFixture, MeTableInheritance)
{
    eActor a(iLua, "tests/scripts/meTableInheritance/C.lua");
    a.doScript();
    a.callOnInit();
    a.callLuaFunc("test1");
    a.callLuaFunc("test2");
    CHECK(true);
}

TEST_FIXTURE(sFixture, ScriptSupport)
{
    eActor a(iLua, "tests/scripts/scriptSupport.lua");
    eActor b(iLua, "tests/scripts/scriptSupport.lua");

    a.doScript();
    b.doScript();

    a.callLuaFunc("setActorFoo");
    b.callLuaFunc("setActorBar");

    a.callLuaFunc("test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, GadgetTest)
{
    eActor a(iLua, "tests/scripts/gadgetTest.lua");
    a.doScript();
    a.callLuaFunc("test");

    CHECK(true);
}

TEST_FIXTURE(sFixture, StateShift)
{
    eActor a(iLua, "tests/scripts/stateShift.lua");
    a.doScript();
    a.update();
    a.update();
    a.callLuaFunc("test");

    CHECK(true);
}
