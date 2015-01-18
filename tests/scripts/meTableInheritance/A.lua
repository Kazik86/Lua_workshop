Class = "A"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function OnInit(me)
    me.a = 1
end

Super.DefState(This, {
    Name = "state_main"
})

EntryState = state_main
