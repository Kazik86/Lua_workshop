Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/parentState/redefineParent/a.lua")

Super.DefState(This, {
    Name = "state_parent",

    Update = function(me)
	me.counter = me.counter - 1
    end
})

function test(me)
    _G.assert(me.counter == -1)
end
