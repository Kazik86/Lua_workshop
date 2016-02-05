Class = "Base"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.gadget = _G.gUnitTest.create(me)
    me.evenValues = {}
end

function onEqualEvent(me)
    me.equalEvent = true
end

function onEvenEvent(me, val)
    local t = me.evenValues
    t[#t + 1] = val
end

function onGreaterEvent(me, t)
    me.currentVal = t.currentVal
    me.delta = t.delta
end

Super.DefState(This, {
    Name = "state_main",
    Gadgets = { "gadget" },

    Enter = function(me)
	me.triggerVal = 3
	me.gadget:setVal(1)
	me.gadget:notifyWhenEqualTo(me.triggerVal)
	me.gadget:onEqual(onEqualEvent)
	me.gadget:onEven(onEvenEvent)
	me.gadget:onGreater(onGreaterEvent)
    end
})

function test(me)
    _G.assert(me.equalEvent == true)
    _G.assert(me.evenValues[1] == 2)
    _G.assert(me.evenValues[2] == 4)
    _G.assert(me.currentVal == 5)
    _G.assert(me.delta == 2)
end

EntryState = state_main
