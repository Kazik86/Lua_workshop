Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/replaceEnv/a.lua")

function test(me)
    _G.assert(ReplaceEnv(Super.foo.foo)(me) == "Derived")
end

