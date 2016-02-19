Class = "Receiver"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.sender = CreateActor(me, "tests/scripts/signals/mute/sender.lua")

    me.signalFoo = Connect(me.sender, "foo", me, onFoo)
    Connect(me.sender, "bar", me, onBar)

    me.fooCntr = 0
    me.barCntr = 0
    me.muteCntr = 0
end

function onFoo(me)
    me.fooCntr = me.fooCntr + 1
end

function onBar(me)
    me.barCntr = me.barCntr + 1
end

Super.DefState(This, {
    Name = "state_mute",

    Update = function(me)
	me.muteCntr = me.muteCntr + 1

	if me.muteCntr == 1 then MuteSignal(me.signalFoo, true) end
	if me.muteCntr == 2 then MuteAllSignals(me, true) end
	if me.muteCntr == 3 then return Shift(me, state_main) end
    end,

    Leave = function(me)
	MuteAllSignals(me, false)
    end
})

function test(me)
    _G.assert(me.fooCntr == 2)
    _G.assert(me.barCntr == 3)
    _G.assert(me.muteCntr == 3)
end

EntryState = state_mute
