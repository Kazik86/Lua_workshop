Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/extendingState/redefineStateBeingExtended/a.lua")

Super.DefState(This, {
    Name = "state_to_extend",

    Enter = function(me)
	me.redefinedEnter = 1
    end,

    Leave = function(me)
	me.redefinedLeave = 1
    end,

    Update = function(me)
	me.redefinedUpdate = 1
	return Shift(me, state_two)
    end
})

Super.DefState(This, {
    Name = "state_two"
})

function test(me)
    _G.assert(me.origEnter == nil)
    _G.assert(me.origLeave == nil)
    _G.assert(me.origUpdate == nil)
    _G.assert(me.redefinedEnter == 1)
    _G.assert(me.redefinedLeave == 1)
    _G.assert(me.redefinedUpdate == 1)
end

EntryState = state_main
