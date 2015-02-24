Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/breakUpdateWhenShiftInExtended/a.lua")

Super.DefState(This, {
    Extends = Super.state_main,

    Update = function(me)
	_G.assert(false)
    end
})

EntryState = state_main
