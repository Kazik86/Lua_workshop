Class = "Base"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

Super.DefState(This, {
    Name = "state_parent",

    Update = function(me)
	me.text = "foo"
    end
})

Super.DefState(This, {
    Name = "state_child",
    Parent = state_parent,

    Update = function(me)
    end
})

EntryState = state_child
