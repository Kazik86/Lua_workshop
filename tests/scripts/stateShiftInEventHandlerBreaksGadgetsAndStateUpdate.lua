Class = "Base"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.g1 = _G.gUnitTest.create(me)
    me.g2 = _G.gUnitTest.create(me)
    me.g1:setVal(0)
    me.g2:setVal(0)
    me.g1:notifyWhenEqualTo(1)
    me.g1:onEqual(onEqual)
end

function onEqual(me)
    return Shift(me, state_two)
end

Super.DefState(This, {
    Name = "state_main",
    Gadgets = { "g1", "g2" },

    Update = function(me)
	_G.assert(false)
    end
})

Super.DefState(This, {
    Name = "state_two",

    Update = function(me)
	_G.assert(false)
    end
})

function test(me)
    _G.assert(me.g1:getVal() == 1)
    _G.assert(me.g2:getVal() == 0)
end

EntryState = state_main
