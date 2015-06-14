Class = "Base"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

Super.DefState(This, {
    Name = "state_to_extend",

    Enter = function(me)
	me.origEnter = 1
    end,

    Leave = function(me)
	me.origLeave = 1
    end,

    Update = function(me)
	me.origUpdate = 1
    end
})

Super.DefState(This, {
    Name = "state_main",
    Extends = state_to_extend
})

EntryState = state_main
