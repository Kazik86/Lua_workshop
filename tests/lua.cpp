#include <UnitTest++/UnitTest++.h>

#include "fsm.h"
#include "luaState.h"

#include <stdexcept>

struct sFixture
{
    eLuaState iLua;
};

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

    CHECK(fsm1.getModuleAsPtr() == fsm2.getModuleAsPtr());
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
    fsm.callLuaFunct("testMetatable");

    CHECK(lua_toboolean(iLua.getRaw(), -1));
}

TEST_FIXTURE(sFixture, Inheritance)
{
    eFsm fsm(iLua, "tests.scripts.base");
    fsm.doScript();
    fsm.callLuaFunct("testInheritance");

    CHECK(lua_toboolean(iLua.getRaw(), -1));
}
