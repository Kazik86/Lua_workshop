#include <UnitTest++/UnitTest++.h>

#include "fsm.h"
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

	lua_pop(lua, 3);
    }
}

TEST_FIXTURE(sFixture, NonExistentScript)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests.scripts.nonexistent"), std::runtime_error);
}

TEST_FIXTURE(sFixture, NoTableReturned)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests.scripts.noTableReturned"), std::runtime_error);
}

TEST_FIXTURE(sFixture, SameScript)
{
    int ref1 = LuaModuleMgr::load(iLua, "tests.scripts.base");
    int ref2 = LuaModuleMgr::load(iLua, "tests.scripts.base");
    
    CHECK(ref1 == ref2);
}

TEST_FIXTURE(sFixture, InvalidClassField1)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests.scripts.invalidClass1"), std::runtime_error);
}

TEST_FIXTURE(sFixture, InvalidClassField2)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests.scripts.invalidClass2"), std::runtime_error);
}

TEST_FIXTURE(sFixture, InvalidClassField3)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests.scripts.invalidClass3"), std::runtime_error);
}

TEST_FIXTURE(sFixture, Metatable)
{
    int ref = LuaModuleMgr::load(iLua, "tests.scripts.base");
    callLuaFun(iLua, ref, "testMetatable", 0, 1);
}

TEST_FIXTURE(sFixture, Inheritance)
{
    int ref = LuaModuleMgr::load(iLua, "tests.scripts.base");
    callLuaFun(iLua, ref, "testInheritance", 0, 1);
}

TEST_FIXTURE(sFixture, ClassUniqueness1)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests.scripts.classUniqueness1.base"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassUniqueness2)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests.scripts.classUniqueness2.base"), std::runtime_error);
}

TEST_FIXTURE(sFixture, ClassUniqueness3)
{
    CHECK_THROW(LuaModuleMgr::load(iLua, "tests.scripts.classUniqueness3.base"), std::runtime_error);
}
