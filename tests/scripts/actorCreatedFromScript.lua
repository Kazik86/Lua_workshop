Class = "Foo"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.child = _G.eActorMgr.add("tests/scripts/meTable.lua")
end

Super.DefState(This, {
    Name = "state_main"
})

EntryState = state_main
