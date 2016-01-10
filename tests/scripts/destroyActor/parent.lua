Class = "Parent"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.child1 = CreateActor(me, "tests/scripts/destroyActor/child.lua")
    me.child2 = CreateActor(me, "tests/scripts/destroyActor/child.lua")
    _G.assert(#me.Children == 2)

    DestroyActor(me, "child1")
    _G.assert(#me.Children == 2)

    DestroyActor(me, "child2")
    _G.assert(#me.Children == 1)
    _G.assert(me.child2 == nil)
end
