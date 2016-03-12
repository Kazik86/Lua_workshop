Class = "Kutas"
Super = _G.eLuaModuleMgr.derive("scripts/transform.lua")


function Init(me)
    me.gTexture = _G.gTexture.create(me)
    me.gTexture:setName("textures/kutas.png")
    me.gTexture:setSdlRectW(49)
    me.gTexture:setSdlRectH(69)
    me.gTexture:enable()

    me.gMove = _G.gMove.create(me)
    me.gRotate = _G.gRotate.create(me)
    me.gTexture:setMoveGadget(me.gMove)
    me.gTexture:setRotateGadget(me.gRotate)

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
    Gadgets = { "gMove", "gRotate" },
})

Super.DefState(This, {
    Name = "state_moveDown",
    Parent = state_moveDown_parent,

    Enter = function(me)
	me.gMove:setSpeed(_G.math.random(100, 200))
	me.gRotate:setOmega(_G.math.random(100, 400))

        if _G.math.random(1,20)>10 then
          me.gRotate:setDir(1)
        else
          me.gRotate:setDir(-1)
        end

	me.gTimer:setInterval(1000)
	me.gTimer:onElapsed(function()
				return Shift(me, state_moveDown_over)
			    end)
    end,

    Update = function(me)
	_, y = me.gTransform:getPos()
	if y > 300 then
	    return Shift(me, state_sleep)
	end
    end
})

Super.DefState(This, {
    Name = "state_moveDown_over",
    Parent = state_moveDown_parent,

    Update = function(me)
	_, y = me.gTransform:getPos()
	if y > 600 then
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
