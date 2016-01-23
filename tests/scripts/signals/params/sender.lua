Class = "Sender"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.foo = CreateActor(me, "tests/scripts/signals/params/foo.lua")

    DefSignal(me, "stateEnter")
    DefSignal(me, "stateLeave")

    Connect(me, "stateEnter", me.foo, _G.Foo.enterHandler)
    Connect(me, "stateLeave", me.foo, _G.Foo.leaveHandler)
end

Super.DefState(This, {
    Name = "state_main",

    Update = function(me)
	return Shift(me, state_two)
    end,

    Leave = function(me)
	Emit(me, "stateLeave", "state_main")
    end
})

Super.DefState(This, {
    Name = "state_two",

    Enter = function(me)
	Emit(me, "stateEnter", { "state_two" })
    end
})

function test(me)
    _G.assert(me.foo.enterParams[1] == "state_two")
    _G.assert(me.foo.leaveParams == "state_main")
end

EntryState = state_main
