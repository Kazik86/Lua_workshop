Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/autoEnableDisableGadgets/a.lua")

function Init(me)
    me.gUnitTest3 = _G.gUnitTest.create(me)
    me.gUnitTest3:setVal(100)

    me.gUnitTest4 = _G.gUnitTest.create(me)
    me.gUnitTest4:setVal(200)
end

Super.DefState(This, {
    Extends = Super.state_gadgetsEnabledEx,
    Gadgets = { "gUnitTest3", "gUnitTest4" },

    Update = function(me)
	_G.assert(me.gUnitTest1:getVal() == 1)
	_G.assert(me.gUnitTest2:getVal() == 1)

	_G.assert(me.gUnitTest3:getVal() == 101)
	_G.assert(me.gUnitTest4:getVal() == 201)

	return Shift(me, state_oneGadgetDisabled)
    end
})

Super.DefState(This, {
    Extends = Super.state_oneGadgetDisabled,
    Gadgets = { "gUnitTest4" },

    Update = function(me)
	_G.assert(me.gUnitTest1:getVal() == 1)
	_G.assert(me.gUnitTest2:getVal() == 2)

	_G.assert(me.gUnitTest3:getVal() == 101)
	_G.assert(me.gUnitTest4:getVal() == 202)
    end
})

function test(me)
    _G.assert(me.gUnitTest1:getVal() == 1)
    _G.assert(me.gUnitTest2:getVal() == 2)

    _G.assert(me.gUnitTest3:getVal() == 101)
    _G.assert(me.gUnitTest4:getVal() == 202)
end

EntryState = state_gadgetsEnabledEx
