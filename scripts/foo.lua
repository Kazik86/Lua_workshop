Class = "Foo"
Derives = "scripts/Actor.lua"

function OnInit(me)
    me.gTexture = _G.gTexture.create()
    me.gTexture:setName("textures/wb_logo_brighter.png")
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
