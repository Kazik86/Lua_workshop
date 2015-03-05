Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/parentState/extendParent/a.lua")

Super.DefState(This, {
    Extends = Super.state_parent,

    Update = function(me)
	me.text = me.text .. " bar"
    end
})

function test(me)
    _G.assert(me.text == "foo bar")
end
