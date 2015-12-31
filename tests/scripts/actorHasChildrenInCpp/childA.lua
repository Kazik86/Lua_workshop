Class = "ChildA"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.childB = CreateActor(me, "tests/scripts/actorHasChildrenInCpp/childB.lua")
end
