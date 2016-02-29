Class = "SplashScreen"
Super = _G.eLuaModuleMgr.derive("scripts/transform.lua")

function Init(me)
    me.splashScreen = _G.gTexture.create(me)
    me.splashScreen:setName("textures/engine.png")
    me.splashScreen:setPosFromActor(false)
    me.splashScreen:setRotFromActor(false)
    me.splashScreen:setSdlRectX(0) me.splashScreen:setSdlRectY(0)
    me.splashScreen:setFadeDuration(1)

    me.gTimer = _G.gTimer.create(me)

    DefSignal(me, "finished")
end

function start(me)
    return Shift(me, state_show)
end

Super.DefState(This, {
    Name = "state_show",
    Gadgets = { "gTimer", "splashScreen" },

    Enter = function(me)
	me.gTimer:start()
	me.splashScreen:fadeOff()
    end,

    Update = function(me)
	if (me.gTimer:isElapsed(2000)) then
	    return Shift(me, state_fadeOut)
	end
    end
})


Super.DefState(This, {
    Name = "state_fadeOut",
    Gadgets = { "splashScreen" },

    Enter = function(me)
	me.splashScreen:fadeOut()
    end,

    Update = function(me)
	if (me.splashScreen:isFadeCompleted()) then
	    return Shift(me, state_finished)
	end
    end
})

Super.DefState(This, {
    Name = "state_finished",

    Enter = function(me)
	Emit(me, "finished")
    end
})
