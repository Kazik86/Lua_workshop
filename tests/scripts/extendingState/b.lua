Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/extendingState/a.lua")

Super.DefState(This, {
    Extends = Super.state_main,

    Enter = function(me)
	me.derivedEnter = 1
    end,

    Leave = function(me)
	me.derivedLeave = 1
    end,

    Update = function(me)
	me.derivedUpdate = 1
	return Shift(me, state_two)
    end
})

Super.DefState(This, {
    Name = "state_two"
})

function test(me)
    _G.assert(me.baseEnter == 1)
    _G.assert(me.baseLeave == 1)
    _G.assert(me.baseUpdate == 1)
    _G.assert(me.derivedEnter == 1)
    _G.assert(me.derivedLeave == 1)
    _G.assert(me.derivedUpdate == 1)
end

EntryState = state_main
