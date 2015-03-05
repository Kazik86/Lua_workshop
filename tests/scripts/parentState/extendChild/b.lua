Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/parentState/extendChild/a.lua")

Super.DefState(This, {
    Extends = Super.state_child,

    Update = function(me)
	me.text = me.text .. "Derived::state_child"
    end
})

function test(me)
    _G.assert(me.text == "parent, Base::state_child, Derived::state_child")
end

EntryState = state_child
