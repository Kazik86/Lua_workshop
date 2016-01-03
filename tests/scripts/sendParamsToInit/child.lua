Class = "Child"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me, params)
    _G.assert(params.foo == "foo")
    _G.assert(params.bar == "bar")
end
