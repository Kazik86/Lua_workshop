Class = "Foo"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function test(me)
    _G.assert(me.Env == _G.Foo)
    _G.assert(me.Env == This)
end
