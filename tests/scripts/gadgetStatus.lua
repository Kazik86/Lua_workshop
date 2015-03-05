Class = "Foo"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.gUnitTest = _G.gUnitTest.create(me)
end

Super.DefState(This, {
    Name = "state_main",
    Gadgets = { "gUnitTest" },

    Enter = function(me)
	me.gUnitTest:setVal(10)
    end
})

function test(me)
    _G.assert(me.gUnitTest:isValGreater(9) == true)
    _G.assert(me.gUnitTest:isValGreater(11) == false)
end

EntryState = state_main
