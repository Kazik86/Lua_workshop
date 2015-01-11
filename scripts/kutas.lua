Class = "Kutas"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")


function OnInit(me)
    me.gTexture = _G.gTexture.create(me)
    me.gTexture:setName("textures/kutas.png")
    me.gTexture:setSdlRectW(49)
    me.gTexture:setSdlRectH(69)
    me.gTexture:enable()

    me.gMove = _G.gMove.create(me)
    me.gRandomPos = _G.gRandomPos.create(me)

    Super.shift(me, state_main)
end


state_main = {
    enter = function(me)
	me.gRandomPos:enable()
    end,

    leave = function(me)
	me.gRandomPos:disable()
    end,

    update = function(me)
	Super.shift(me, state_moveDown)
    end
}

state_moveDown = {
    enter = function(me)
	me.gMove:enable()
	me.gMove:setSpeed(_G.math.random(100, 200))
    end,

    leave = function(me)
	me.gMove:disable()
    end,

    update = function(me)
	if (_G.eActor.getY(me.eActor) > 600) then
	    Super.shift(me, state_main)
	end
    end
}


