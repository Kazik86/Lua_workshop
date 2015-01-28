Class = "A"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.a = 1
end

function testInA_1(me)
    _G.assert(me.a == 1)
    _G.assert(me.A.a == 1)
    _G.assert(me.c == 3)
end

function testInA_2(me)
    _G.assert(me.a == 10)
    _G.assert(me.A.a == 1)
end

Super.DefState(This, {
    Name = "state_main"
})

EntryState = state_main
