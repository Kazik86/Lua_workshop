Class = "Base"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

Super.DefState(This, {
    Name = "state_main",

    Enter = function(me)
	me.baseEnter = 1
    end,

    Leave = function(me)
	me.baseLeave = 1
    end,

    Update = function(me)
	me.baseUpdate = 1
    end
})

EntryState = state_main
