Class = "Kutas"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")


function Init(me)
    me.gTexture = _G.gTexture.create(me)
    me.gTexture:setName("textures/kutas.png")
    me.gTexture:setSdlRectW(49)
    me.gTexture:setSdlRectH(69)
    me.gTexture:enable()

    me.gMove = _G.gMove.create(me)
    me.gTexture:setMoveGadget(me.gMove)

    me.gRandomPos = _G.gRandomPos.create(me)

    me.gTimer = _G.gTimer.create(me)
end


Super.DefState(This, {
    Name = "state_main",
    Gadgets = { "gRandomPos" },

    Update = function(me)
	return Shift(me, state_moveDown)
    end
})

Super.DefState(This, {
    Name = "state_moveDown_parent",
    Gadgets = { "gMove" },
})

Super.DefState(This, {
    Name = "state_moveDown",
    Parent = state_moveDown_parent,

    Enter = function(me)
	me.gMove:setSpeed(_G.math.random(100, 200))
	me.gTimer:setInterval(1000)
	me.gTimer:onElapsed(function()
				return Shift(me, state_moveDown_over)
			    end)
    end,

    Update = function(me)
	if (_G.eActor.getY(me.eActor) > 300) then
	    return Shift(me, state_sleep)
	end
    end
})

Super.DefState(This, {
    Name = "state_moveDown_over",
    Parent = state_moveDown_parent,

    Update = function(me)
	if (_G.eActor.getY(me.eActor) > 600) then
	    return Shift(me, state_main)
	end
    end
})

Super.DefState(This, {
    Name = "state_sleep",
    Gadgets = { "gTimer" },

    Enter = function(me)
	me.gTimer:start()
    end
})

EntryState = state_main
