Class = "Base"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.gadget = _G.gUnitTest.create(me)
end

function onEqualEvent(me)
    me.equalEvent = true
end

Super.DefState(This, {
    Name = "state_main",
    Gadgets = { "gadget" },

    Enter = function(me)
	me.triggerVal = 3
	me.gadget:setVal(1)
	me.gadget:notifyWhenEqualTo(me.triggerVal)
	me.gadget:onEqual(onEqualEvent)
    end
})

function test(me)
    _G.assert(me.equalEvent == true)
end

EntryState = state_main