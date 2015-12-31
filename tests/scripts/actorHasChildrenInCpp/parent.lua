Class = "Parent"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.childA = CreateActor(me, "tests/scripts/actorHasChildrenInCpp/childA.lua")
    me.childB = CreateActor(me, "tests/scripts/actorHasChildrenInCpp/childB.lua")
end
