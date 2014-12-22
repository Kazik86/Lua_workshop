Class = "Foo"
Derives = "scripts/Actor.lua"

function OnInit(me)
    me.child = _G.eActorMgr.newActor("tests/scripts/meTable.lua")
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
