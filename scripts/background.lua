Class = "Background"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.gTexture = _G.gTexture.create(me)
    me.gTexture:setName("textures/background.png")
    me.gTexture:setSdlRectW(800)
    me.gTexture:setSdlRectH(600)
    me.gTexture:enable()
end

Super.DefState(This, {
    Name = "state_main"
})

EntryState = state_main
