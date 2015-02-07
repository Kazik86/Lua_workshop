Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/replaceEnv/a.lua")

Super.DefState(This, {
    Name = "state_main",
    Update = function(me)
	_G.assert(ReplaceEnv(Super.foo.foo)(me) == "Derived")
    end
})

EntryState = state_main
