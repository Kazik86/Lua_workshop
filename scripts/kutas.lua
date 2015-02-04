Class = "Kutas"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")


function Init(me)
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
    Gadgets = { "gRandomPos" },

    Update = function(me)
	return Shift(me, state_moveDown)
    end
})

Super.DefState(This, {
    Name = "state_moveDown",
    Gadgets = { "gMove" },

    Enter = function(me)
	me.gMove:setSpeed(_G.math.random(100, 200))
    end,

    Update = function(me)
	if (_G.eActor.getY(me.eActor) > 600) then
	    return Shift(me, state_main)
	end
    end
})

EntryState = state_main
