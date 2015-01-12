Class = "Main"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function OnInit(me)
    me.background = _G.eActorMgr.add("scripts/background.lua")

    for i = 1, 20 do
        _G.eActorMgr.add("scripts/kutas.lua")
        _G.eActorMgr.add("scripts/bigKutas.lua")
    end
end

state_main = {
    enter = function(me)
    end,

    leave = function(me)
    end,

    update = function(me)
    end
}

EntryState = state_main
