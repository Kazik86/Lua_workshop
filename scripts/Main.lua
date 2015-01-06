Class = "Main"
Derives = "scripts/Actor.lua"

function OnInit(me)
    me.background = _G.eActorMgr.add("scripts/background.lua")
    me.kutas = _G.eActorMgr.add("scripts/kutas.lua")
    me.bigKutas = _G.eActorMgr.add("scripts/bigKutas.lua")
    shift(me, state_main)
end

state_main = {
    enter = function(me)
    end,

    leave = function(me)
    end,

    update = function(me)
    end
}
