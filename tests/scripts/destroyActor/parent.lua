Class = "Parent"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.child1 = CreateActor(me, "tests/scripts/destroyActor/child.lua")
    me.child2 = CreateActor(me, "tests/scripts/destroyActor/child.lua")
    _G.assert(#me.Children == 2)

    DestroyActor(me, "child1")
    _G.assert(#me.Children == 2)

    -- weak table
    me.weak = { __mode = "kv" }
    _G.setmetatable(me.weak, me.weak)
    me.weak.child2 = me.child2

    DestroyActor(me, "child2")
    _G.assert(#me.Children == 1)
    _G.assert(me.child2 == nil)

    -- when object referenced by value (or key) in a weak table is collected by
    -- garbage collector, corresponding entry in the table is automatically
    -- removed. (Programming in Lua, 3rd ed., 17.1)
    -- This test makes me sure that destroyed actor was properly collected by GC.
    _G.assert(me.weak.child2 ~= nil)
    _G.collectgarbage()
    _G.assert(me.weak.child2 == nil)
end
