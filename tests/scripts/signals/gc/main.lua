Class = "Main"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.persistent = CreateActor(me, "tests/scripts/signals/gc/persistent.lua")
    me.temporary = CreateActor(me, "tests/scripts/signals/gc/temporary.lua")

    Connect(me.persistent, "foo", me.temporary, _G.Temporary.fooHandler)
end

function test(me)
    -- weak table
    me.weak = { __mode = "kv" }
    _G.setmetatable(me.weak, me.weak)
    me.weak.temp = me.temporary

    _G.assert(me.weak.temp ~= nil)

    -- connecting actor with signal sender shouldn't block Garbage Collector
    -- from collecting actor after being destroyed
    DestroyActor(me, "temporary")
    _G.collectgarbage()
    _G.assert(me.weak.temp == nil)
end

