Class = "Base"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

Super.DefState(This, {
    Name = "state_main",

    Update = function(me)
	return Shift(me, state_child)
    end
})

Super.DefState(This, {
    Name = "state_parent",
    Parent = state_parent,

    Enter = function(me)
	me.parentEnter = 1
    end,

    Leave = function(me)
	me.parentLeave = 1
    end,

    Update = function(me)
	me.parentUpdate = 1
    end
})

Super.DefState(This, {
    Name = "state_child",
    Parent = state_parent,

    Enter = function(me)
	me.childEnter = 1
    end,

    Leave = function(me)
	me.childLeave = 1
    end,

    Update = function(me)
	me.childUpdate = 1
	return Shift(me, state_finish)
    end
})

Super.DefState(This, {
    Name = "state_finish"
})

function test(me)
    _G.assert(me.parentEnter == 1)
    _G.assert(me.parentLeave == 1)
    _G.assert(me.parentUpdate == 1)
    _G.assert(me.childEnter == 1)
    _G.assert(me.childLeave == 1)
    _G.assert(me.childUpdate == 1)
end

EntryState = state_main
