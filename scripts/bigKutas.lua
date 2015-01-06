Class = "BigKutas"
Derives = "scripts/kutas.lua"

function OnInit(me)
    me.gTexture:setName("textures/big_kutas.png")
    me.gTexture:setSdlRectW(98)
    me.gTexture:setSdlRectH(138)
end

function onEnterMoveDown(me)
    Kutas.onEnterMoveDown(me)
    me.gMove:setSpeed(_G.math.random(200, 300))
end

