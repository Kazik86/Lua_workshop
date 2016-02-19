Class = "Sender"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    DefSignal(me, "foo")
    DefSignal(me, "bar")
end

Super.DefState(This, {
    Name = "state_emit",

    Update = function(me)
	Emit(me, "foo")
	Emit(me, "bar")
    end
})

EntryState = state_emit
