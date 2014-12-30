Class = "Baz"
Derives = "scripts/foo.lua"

function OnInit(me)
    me.gTexture:setName("textures/qt.png")
    me.gTexture:setSdlRectX(20)
    me.gTexture:setSdlRectY(20)
    me.gTexture:setSdlRectW(48)
    me.gTexture:setSdlRectH(48)
    shift(me, state_main)
end

state_main = {
    enter = function(me)
    end,

    leave = function(me)
    end,

    update = function(me)
	_G.print "Baz actor"
	local x = me.gTexture:getSdlRectX()
	local y = me.gTexture:getSdlRectY()

	x = x + 1
	y = y + 1

	me.gTexture:setSdlRectX(x)
	me.gTexture:setSdlRectY(y)
    end
}

