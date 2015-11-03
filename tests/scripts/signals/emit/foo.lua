Class = "Foo"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function enterHandler(me)
    me.enter = true
end

function leaveHandler(me)
    me.leave = true
end

