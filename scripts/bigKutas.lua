Class = "BigKutas"
Super = _G.eLuaModuleMgr.derive("scripts/kutas.lua")

function OnInit(me)
    me.gTexture:setName("textures/big_kutas.png")
    me.gTexture:setSdlRectW(98)
    me.gTexture:setSdlRectH(138)
end

Super.DefState(This, {
    Extends = Super.state_moveDown,

    Enter = function(me)
	me.gMove:setSpeed(_G.math.random(200, 300))
    end
})

