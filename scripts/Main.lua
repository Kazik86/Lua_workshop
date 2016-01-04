Class = "Main"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

--input = _G.require "scripts/input"


function Init(me)
    me.DumpState = 1
    me.splashScreen = _G.gTexture.create(me)
    me.splashScreen:setName("textures/engine.png")
    me.splashScreen:setPosFromActor(false)
    me.splashScreen:setRotFromActor(false)
    me.splashScreen:setSdlRectX(0) me.splashScreen:setSdlRectY(0)
    me.splashScreen:setFadeDuration(1)

    me.gFpsCounter = _G.gFpsCounter.create(me)
    me.gFpsCounter:enable()

    me.gTimer = _G.gTimer.create(me)

    -- Menu
    me.menu = CreateActor(me, "scripts/Menu.lua")
    Disable(me.menu)
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
	    return Shift(me, state_menu)
	end
    end
})


Super.DefState(This, {
    Name = "state_menu",

    Enter = function(me)
	Enable(me.menu)
    end
})


EntryState = state_splashScreen
