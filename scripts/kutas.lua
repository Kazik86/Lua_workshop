Class = "Kutas"
Derives = "scripts/Actor.lua"


function OnInit(me)
    me.gTexture = _G.gTexture.create(me)
    me.gTexture:setName("textures/kutas.png")
    me.gTexture:setSdlRectW(49)
    me.gTexture:setSdlRectH(69)
    me.gTexture:enable()

    me.gMove = _G.gMove.create(me)
    --me.gRandomPos = _G.gRandomPos.create(me)

    shift(me, state_main)
end


state_main = {
    enter = function(me)
	--me.gRandomPos:enable()
    end,

    leave = function(me)
	--me.gRandomPos:disable()
    end,

    update = function(me)
	shift(me, state_moveDown)
    end
}


function onEnterMoveDown(me)
    me.gMove:enable()
    me.gMove:setSpeed(30)
end

state_moveDown = {
    enter = function(me)
	onEnterMoveDown(me)
    end,

    leave = function(me)
	me.gMove:disable()
    end,

    update = function(me)
	--if (getY(me) > 640) then
	--    shift(me, state_main)
	--end
    end
}


