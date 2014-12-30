Class = "Foo"
Derives = "scripts/Actor.lua"

function OnInit(me)
    me.gTexture = _G.gTexture.create(me)
    shift(me, state_main)
end

state_main = {
    enter = function(me)
	me.gTexture:enable()
    end,

    leave = function(me)
    end,

    update = function(me)
	_G.print "Foo actor"
    end
}
