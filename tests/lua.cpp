#include <UnitTest++/UnitTest++.h>

#include "fsm.h"
#include "luaState.h"

#include <stdexcept>

namespace
{
    struct sFixture
    {
	eLuaState iLua;
    };

    // [-0, +0]
    void callLuaFun(eLuaState& aLua, eFsm& aFsm, const char* aName, int aArgsNum, int aResultsNum)
    {
	lua_State* lua = aLua.getRaw();
	int moduleRef = aFsm.getModuleRef();

	lua_rawgeti(lua, LUA_REGISTRYINDEX, moduleRef);
	lua_getfield(lua, -1, aName);
	lua_pcall(lua, aArgsNum, aResultsNum, 0);

	CHECK(lua_toboolean(lua, -1));

	lua_pop(lua, 3);
    }
}

TEST_FIXTURE(sFixture, NonExistentScript)
{
    eFsm fsm (iLua, "tests.scripts.nonexistent");
    CHECK_THROW(fsm.doScript(), std::runtime_error);
}

TEST_FIXTURE(sFixture, NoTableReturned)
{
    eFsm fsm(iLua, "tests.scripts.noTableReturned");
    CHECK_THROW(fsm.doScript(), std::runtime_error);
}

TEST_FIXTURE(sFixture, SameScript)
{
    eFsm fsm1(iLua, "tests.scripts.base");
    eFsm fsm2(iLua, "tests.scripts.base");
    
    fsm1.doScript();
    fsm2.doScript();

    lua_State* lua = iLua.getRaw();

    lua_rawgeti(lua, LUA_REGISTRYINDEX, fsm1.getModuleRef());
    const void* fsm1Module = lua_topointer(lua, -1);
    lua_pop(lua, 1);

    lua_rawgeti(lua, LUA_REGISTRYINDEX, fsm2.getModuleRef());
    const void* fsm2Module = lua_topointer(lua, -1);
    lua_pop(lua, 1);

    CHECK(fsm1Module == fsm2Module);
}

TEST_FIXTURE(sFixture, InvalidClassField1)
{
    eFsm fsm(iLua, "tests.scripts.invalidClass1");
    CHECK_THROW(fsm.doScript(), std::runtime_error);
}

TEST_FIXTURE(sFixture, InvalidClassField2)
{
    eFsm fsm(iLua, "tests.scripts.invalidClass2");
    CHECK_THROW(fsm.doScript(), std::runtime_error);
}

TEST_FIXTURE(sFixture, InvalidClassField3)
{
    eFsm fsm(iLua, "tests.scripts.invalidClass3");
    CHECK_THROW(fsm.doScript(), std::runtime_error);
}

TEST_FIXTURE(sFixture, Metatable)
{
    eFsm fsm(iLua, "tests.scripts.base");
    fsm.doScript();
    callLuaFun(iLua, fsm, "testMetatable", 0, 1);
}

TEST_FIXTURE(sFixture, Inheritance)
{
    eFsm fsm(iLua, "tests.scripts.base");
    fsm.doScript();
    callLuaFun(iLua, fsm, "testInheritance", 0, 1);
}
