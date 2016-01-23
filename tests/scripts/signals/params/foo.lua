Class = "Foo"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function enterHandler(me, params)
    me.enterParams = params
end

function leaveHandler(me, params)
    me.leaveParams = params
end

