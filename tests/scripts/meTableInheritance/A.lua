Class = "A"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function OnInit(me)
    me.a = 1
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
