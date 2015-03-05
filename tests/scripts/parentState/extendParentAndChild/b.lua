Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/parentState/extendParentAndChild/a.lua")

Super.DefState(This, {
    Extends = Super.state_parent,

    Update = function(me)
	me.text = me.text .. "b"
    end
})

Super.DefState(This, {
    Extends = Super.state_child,

    Update = function(me)
	me.text = me.text .. "d"
    end
})

function test(me)
    _G.assert(me.text == "abcd")
end

EntryState = state_child
