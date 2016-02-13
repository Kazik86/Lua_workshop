Class = "Input"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

local actions = { up =    { signal = "up",    flags = {}, status = false }, -- up arrow
		  down =  { signal = "down",  flags = {}, status = false }, -- down arrow
		  left =  { signal = "left" , flags = {}, status = false }, -- left arrow
		  right = { signal = "right", flags = {}, status = false }, -- right arrow
		  lmb =   { signal = "lmb",   flags = {}, status = false }, -- left mouse button
		  rmb =   { signal = "rmb",   flags = {}, status = false }, -- right mouse button
		  mmb =   { signal = "mmb",   flags = {}, status = false }, -- middle mouse button
		  wheel = { signal = "wheel", flags = {}, status = false }, -- mouse wheel
		  mouseMove = { signal = "mouseMove", flags = {}, status = false }, -- mouse moving
		}

local function updateActionStatus(action, eventData)
    local fs = action.flags

    -- _G.print "flags: "
    -- for i = 1, #fs do
    --     _G.print("idx: ", i, " val: ", fs[i])
    -- end

    for i = 1, #fs do
	if fs[i] then
	    if action.status == false then
		action.status = true
		Emit(_G.Input.me, action.signal, eventData)
	    end
	    return
	end
    end

    action.status = false
end

local function setActionFlag(action, flag, eventData)
    -- _G.print("setActionFlag " .. flag)

    local fs = action.flags
    local f = flag ~= 0

    for i = 1, #fs do
	if fs[i] == not f then
	    fs[i] = f
	    updateActionStatus(action, eventData)
	    return
	end
    end

    fs[#fs + 1] = f
    updateActionStatus(action, eventData)
end


-- keyboard -----------------------------------------------------------

-- based on https://wiki.libsdl.org/SDLScancodeLookup
local keys = { { action = actions.right, scancode = 79 },
	       { action = actions.left,  scancode = 80 },
	       { action = actions.down,  scancode = 81 },
	       { action = actions.up,    scancode = 82 },
	     }

local function getActionFromKey(scancode)
    for i = 1, #keys do
	if scancode == keys[i].scancode then
	    return keys[i].action
	end
    end
end

local function onKey(me, data)
    -- _G.print("state: ", data.state)

    local action = getActionFromKey(data.scancode)
    if action then setActionFlag(action, data.state) end
end


-- mouse --------------------------------------------------------------

-- based on 'SDL_mouse.h'
local mouseButtons = { { action = actions.lmb, code = 1 },
                       { action = actions.mmb, code = 2 },
		       { action = actions.rmb, code = 3 },
		     }

local function getActionFromButton(code)
    for i = 1, #mouseButtons do
	if code == mouseButtons[i].code then
	    return mouseButtons[i].action
	end
    end
end

local function onMouseButton(me, data)
    setActionFlag(getActionFromButton(data.button), data.state, data)
end

local function onMouseMove(me, data)
    setActionFlag(actions.mouseMove, data.state)
end

local function onMouseWheel(me, data)
    setActionFlag(actions.wheel, data.state, data)
end


-- api ----------------------------------------------------------------

function action(name)
    return actions[name].status
end


-- helpers ------------------------------------------------------------

local function defineSignals(me)
    for _, v in _G.pairs(actions) do
	DefSignal(me, v.signal)
    end
end


function Init(me)
    _G.Input.me = me

    me.sdlInput = _G.gSdlInput.create(me)
    me.sdlInput:keyboard(onKey)
    me.sdlInput:mouseButton(onMouseButton)
    me.sdlInput:mouseMove(onMouseMove)
    me.sdlInput:mouseWheel(onMouseWheel)
    me.sdlInput:enable()

    defineSignals(me)
end

