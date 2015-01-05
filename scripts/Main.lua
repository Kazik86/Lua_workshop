Class = "Main"
Derives = "scripts/Actor.lua"

function OnInit(me)
    me.bar = _G.eActorMgr.add("scripts/bar.lua")
    me.baz = _G.eActorMgr.add("scripts/baz.lua")
    shift(me, state_main)
end

state_main = {
    enter = function(me)
    end,

    leave = function(me)
    end,

    update = function(me)
	_G.print "main"
    end
}
