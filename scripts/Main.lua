Class = "Main"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.background = _G.eActorMgr.add("scripts/background.lua")

    for i = 1, 50 do
        _G.eActorMgr.add("scripts/kutas.lua")
    --    _G.eActorMgr.add("scripts/bigKutas.lua")
    end

    --me.gFpsCounter = _G.gFpsCounter.create(me)
    --me.gFpsCounter:enable()
end

Super.DefState(This, {
    Name = "state_main",
})

EntryState = state_main
