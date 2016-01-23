Class = "Persistent"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    DefSignal(me, "foo")
end

