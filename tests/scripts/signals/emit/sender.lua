Class = "Sender"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.foo = CreateActor(me, "tests/scripts/signals/emit/foo.lua")
    me.bar = CreateActor(me, "tests/scripts/signals/emit/bar.lua")

    me.disabled = CreateActor(me, "tests/scripts/signals/emit/bar.lua")
    Disable(me.disabled)

    DefSignal(me, "stateEnter")
    DefSignal(me, "stateLeave")

    Connect(me, "stateEnter", me.foo, _G.Foo.enterHandler)
    Connect(me, "stateLeave", me.foo, _G.Foo.leaveHandler)
    Connect(me, "stateEnter", me.bar, _G.Bar.enterHandler)
    Connect(me, "stateLeave", me.bar, _G.Bar.leaveHandler)
    Connect(me, "stateEnter", me.disabled, _G.Bar.enterHandler)
    Connect(me, "stateLeave", me.disabled, _G.Bar.leaveHandler)
end

Super.DefState(This, {
    Name = "state_main",

    Update = function(me)
	return Shift(me, state_two)
    end,

    Leave = function(me)
	Emit(me, "stateLeave")
    end
})

Super.DefState(This, {
    Name = "state_two",

    Enter = function(me)
	Enable(me.disabled)
	Emit(me, "stateEnter")
    end
})

function test(me)
    _G.assert(me.foo.enter == true)
    _G.assert(me.foo.leave == true)
    _G.assert(me.bar.enter == true)
    _G.assert(me.bar.leave == true)

    -- disabled actor shouldn't receive any signal
    _G.assert(me.disabled.enter == true)
    _G.assert(me.disabled.leave == nil)
end

EntryState = state_main
