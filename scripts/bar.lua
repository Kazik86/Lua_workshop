Class = "Bar"
Derives = "scripts/foo.lua"

function OnInit(me)
    me.gTexture:setName("textures/PNG_transparency_demonstration_2.png")
    me.gTexture:setSdlRectX(0)
    me.gTexture:setSdlRectY(0)
    me.gTexture:setSdlRectW(800)
    me.gTexture:setSdlRectH(600)
    shift(me, state_main)
end

state_main = {
    enter = function(me)
    end,

    leave = function(me)
    end,

    update = function(me)
	_G.print "Bar actor"
	local w = me.gTexture:getSdlRectW()
	local h = me.gTexture:getSdlRectH()

	w = w - 1
	h = h - 1

	me.gTexture:setSdlRectW(w)
	me.gTexture:setSdlRectH(h)
    end
}
