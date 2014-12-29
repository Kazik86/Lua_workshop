Class = "Bar"
Derives = "scripts/foo.lua"

function OnInit(me)
    me.gTexture:setName("textures/wb_logo_brighter.png")
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
