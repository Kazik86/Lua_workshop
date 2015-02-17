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
    end,

    Update = function(me)
	if me.gadget:getVal() == me.triggerVal then
	    _G.assert(me.equalEvent == true)
	end
    end
})

EntryState = state_main
