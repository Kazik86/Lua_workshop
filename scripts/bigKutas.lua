Class = "BigKutas"
Super = _G.eLuaModuleMgr.derive("scripts/kutas.lua")

function OnInit(me)
    me.gTexture:setName("textures/big_kutas.png")
    me.gTexture:setSdlRectW(98)
    me.gTexture:setSdlRectH(138)
end

state_moveDown = {
    enter = function(me)
	Super.state_moveDown.enter(me)
	me.gMove:setSpeed(_G.math.random(200, 300))
    end,

    leave =  Super.state_moveDown.leave,
    update = Super.state_moveDown.update
}

