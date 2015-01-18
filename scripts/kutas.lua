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
end


Super.DefState(This, {
    Name = "state_main",

    Enter = function(me)
	me.gRandomPos:enable()
    end,

    Leave = function(me)
	me.gRandomPos:disable()
    end,

    Update = function(me)
	Super.shift(me, state_moveDown)
    end
})

Super.DefState(This, {
    Name = "state_moveDown",

    Enter = function(me)
	me.gMove:enable()
	me.gMove:setSpeed(_G.math.random(100, 200))
    end,

    Leave = function(me)
	me.gMove:disable()
    end,

    Update = function(me)
	if (_G.eActor.getY(me.eActor) > 600) then
	    Super.shift(me, state_main)
	end
    end
})

EntryState = state_main
