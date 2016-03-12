Class = "Main"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")


function Init(me)
    me.input = CreateActor(me, "scripts/input.lua")

    me.splashScreen = CreateActor(me, "scripts/splashScreen.lua")
    Connect(me.splashScreen, "finished", me, function(me) return Shift(me, state_idle) end)

    CreateActor(me, "scripts/background.lua")
    for i = 1, 50 do
	CreateActor(me, "scripts/kutas.lua")
    end

    me.gFpsCounter = _G.gFpsCounter.create(me)
    me.gFpsCounter:setZOrder(1)
    me.gFpsCounter:enable()

    me.gTimer = _G.gTimer.create(me)
end

Super.DefState(This, {
    Name = "state_splashScreen",

    Enter = function(me)
	me.splashScreen.Env.start(me.splashScreen)
    end
})


Super.DefState(This, {
    Name = "state_idle"
})


EntryState = state_splashScreen
