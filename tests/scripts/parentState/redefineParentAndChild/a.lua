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

    Update = function(me)
	_G.assert(false)
    end
})

Super.DefState(This, {
    Name = "state_child",
    Parent = state_parent,

    Update = function(me)
	_G.assert(false)
    end
})

EntryState = state_main
