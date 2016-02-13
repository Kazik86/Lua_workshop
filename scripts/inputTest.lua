Class = "InputTest"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.DumpState = 1

    -- available actions are listed in 'actions' table in 'scripts/input.lua'
    Connect(_G.Input.me, "up", me, up)
    Connect(_G.Input.me, "lmb", me, lmb)
    Connect(_G.Input.me, "mmb", me, mmb)
    Connect(_G.Input.me, "rmb", me, rmb)
    Connect(_G.Input.me, "mouseMove", me, mouseMove)
    Connect(_G.Input.me, "wheel", me, wheel)
end

function up(me)
    _G.print("up pressed")
    return Shift(me, state_moveUp)
end

function lmb(me, data)
    _G.print("lmb pressed, x: ", data.x, ", y: ", data.y)
end

function mmb(me, data)
    _G.print("mmb pressed, x: ", data.x, ", y: ", data.y)
end

function rmb(me, data)
    _G.print("rmb pressed, x: ", data.x, ", y: ", data.y)
end

function mouseMove(me)
    _G.print("mouse is moving")
    return Shift(me, state_mouseMove)
end

function wheel(me, data)
    _G.print("mouse wheel, x: ", data.x, ", y: ", data.y)
end

Super.DefState(This, {
    Name = "state_moveUp",

    Update = function(me)
	if not _G.Input.action("up") then
	    return Shift(me, state_main)
	end
    end
})

Super.DefState(This, {
    Name = "state_mouseMove",

    Update = function(me)
	if not _G.Input.action("mouseMove") then
	    return Shift(me, state_main)
	end
    end
})
