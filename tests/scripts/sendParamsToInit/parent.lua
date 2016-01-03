Class = "Parent"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.child = CreateActor(me, "tests/scripts/sendParamsToInit/child.lua", { foo = "foo", bar = "bar" })
end
