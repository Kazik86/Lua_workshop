Class = "Base"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.counter = 0
end

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
    end
})

EntryState = state_child
