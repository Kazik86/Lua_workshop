Class = "Main"
Derives = "scripts/Actor.lua"

function OnInit(me)
    me.foo = _G.eActorMgr.newActor("scripts/foo.lua")
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
