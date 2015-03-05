Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/parentState/redefineChild/a.lua")

Super.DefState(This, {
    Name = "state_child",
    Parent = Super.state_parent,

    Update = function(me)
	me.child = me.child + 1
    end
})

function test(me)
    _G.assert(me.parent == 1)
    _G.assert(me.child == 1)
end

EntryState = state_child
