Class = "Base"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.parent = 0
    me.child = 0
end

Super.DefState(This, {
    Name = "state_parent",

    Update = function(me)
	me.parent = me.parent + 1
    end
})

Super.DefState(This, {
    Name = "state_child",
    Parent = state_parent,

    Update = function(me)
	_G.assert(false)
    end
})

EntryState = state_child
