Class = "Main"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")


function Init(me)
    me.splashScreen = _G.gTexture.create(me)
    me.splashScreen:setName("textures/engine.png")
    me.splashScreen:setPosFromActor(false)
    me.splashScreen:setRotFromActor(false)
    me.splashScreen:setSdlRectW(800)
    me.splashScreen:setSdlRectH(480)
    me.splashScreen:setSdlRectX(0) me.splashScreen:setSdlRectY(0)
    me.splashScreen:setFadeDuration(1)

    me.gFpsCounter = _G.gFpsCounter.create(me)
    me.gFpsCounter:enable()

    me.gTimer = _G.gTimer.create(me)
end


Super.DefState(This, {
    Name = "state_splashScreen",
    Gadgets = { "gTimer", "splashScreen" },

    Enter = function(me)
	me.gTimer:start()
    end,

    Update = function(me)
	if (me.gTimer:isElapsed(2000)) then
	    return Shift(me, state_splashScreen_fadeOut)
	end
    end
})


Super.DefState(This, {
    Name = "state_splashScreen_fadeOut",
    Gadgets = { "splashScreen" },

    Enter = function(me)
	me.splashScreen:fadeOut()
    end,

    Update = function(me)
	if (me.splashScreen:isFadeCompleted()) then
	    return Shift(me, state_idle)
	end
    end
})


Super.DefState(This, {
    Name = "state_idle"
})


EntryState = state_splashScreen
