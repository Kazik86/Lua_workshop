Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/virtualFunctionInsideState/a.lua")

Super.DefState(This, {
    Name = "state_two",

    Update = function(me)
	me.foo = 4
    end
})

function test(me)
    _G.assert(me.foo == 4)
end

EntryState = state_main
