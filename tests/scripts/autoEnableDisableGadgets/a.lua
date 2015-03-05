Class = "Base"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.gUnitTest1 = _G.gUnitTest.create(me)
    me.gUnitTest1:setVal(0)

    me.gUnitTest2 = _G.gUnitTest.create(me)
    me.gUnitTest2:setVal(0)
end

Super.DefState(This, {
    Name = "state_gadgetsEnabled",
    Gadgets = { "gUnitTest1", "gUnitTest2" },

    Update = function(me)
	_G.assert(me.gUnitTest1:getVal() == 1)
	_G.assert(me.gUnitTest2:getVal() == 1)

	return Shift(me, state_oneGadgetDisabled)
    end
})

Super.DefState(This, {
    Name = "state_oneGadgetDisabled",
    Gadgets = { "gUnitTest2" },

    Update = function(me)
	_G.assert(me.gUnitTest1:getVal() == 1)
	_G.assert(me.gUnitTest2:getVal() == 2)
    end
})

function test(me)
    _G.assert(me.gUnitTest1:getVal() == 1)
    _G.assert(me.gUnitTest2:getVal() == 2)
end

-- used in AutoEnableDisableGadgets2

Super.DefState(This, {
    Name = "state_gadgetsEnabledEx",
    Gadgets = { "gUnitTest1", "gUnitTest2" }
})

EntryState = state_gadgetsEnabled
