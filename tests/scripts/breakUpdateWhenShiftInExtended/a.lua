Class = "Base"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

Super.DefState(This, {
    Name = "state_main",

    Update = function(me)
	return Shift(me, state_two)
    end
})

Super.DefState(This, {
    Name = "state_two"
})

EntryState = state_main
