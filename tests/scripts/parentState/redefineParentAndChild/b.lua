Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/parentState/redefineParentAndChild/a.lua")

Super.DefState(This, {
    Name = "state_parent",

    Update = function(me)
	me.text = "parent "
    end
})

Super.DefState(This, {
    Name = "state_child",
    Parent = Super.state_parent,

    Update = function(me)
	me.text = me.text .. "child"
    end
})

function test(me)
    _G.assert(me.text == "parent child")
end

