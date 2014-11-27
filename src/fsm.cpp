#include "fsm.h"

#include "luaModule.h"
#include "luaState.h"

eFsm::eFsm(eLuaState& aLua, const std::string& aScript):
    iLua(aLua),
    iScript(aScript),
    iModule(LUA_NOREF),
    iEnv(LUA_NOREF)
{
}

eFsm::~eFsm()
{
    luaL_unref(iLua.getRaw(), LUA_REGISTRYINDEX, iEnv);
}

void eFsm::doScript()
{
    iModule = LuaModuleMgr::load(iLua, iScript);
}

void eFsm::update()
{

}

