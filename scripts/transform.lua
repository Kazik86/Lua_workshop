Class = "Transform"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.gTransform = _G.gTransform.create(me)
end
