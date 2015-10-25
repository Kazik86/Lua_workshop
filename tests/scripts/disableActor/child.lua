Class = "Child"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

Super.DefState(This, {
    Name = "state_main"
})

Super.DefState(This, {
    Extends = Super.state_Disabled,

    Update = function(me)
	_G.assert(false)
    end
})

EntryState = state_main
