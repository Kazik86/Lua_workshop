Class = "Actor"

function Init(me)
    --me.DumpState = 1
end

function Shift(me, newState)
    if (me.DumpState ~= nil and me.State ~= nil) then
	_G.print(Class .. " is about to leave " .. me.State.FullName)
    end

    _G.eActor.shift(me.eActor, newState)
    me.State = newState

    if (me.DumpState ~= nil) then
	_G.print(Class .. " entered " .. me.State.FullName)
    end

    return 1
end

function EnableGadgets(me, gadgets)
    if gadgets ~= nil then
	for _, g in _G.ipairs(gadgets) do
	    me[g]:enable()
	end
    end
end

function DisableGadgets(me, gadgets)
    if gadgets ~= nil then
	for _, g in _G.ipairs(gadgets) do
	    me[g]:disable()
	end
    end
end

function DefEnterExNoExtend(t)
    if t.Enter == nil then
	t.EnterEx = function(me)
	    EnableGadgets(me, t.Gadgets)
	end
    else
	t.EnterEx = function(me)
	    EnableGadgets(me, t.Gadgets)
	    ReplaceEnv(t.Enter)(me)
	end
    end
end

function DefLeaveExNoExtend(t)
    if t.Leave == nil then
	t.LeaveEx = function(me)
	    DisableGadgets(me, t.Gadgets)
	end
    else
	t.LeaveEx = function(me)
	    ReplaceEnv(t.Leave)(me)
	    DisableGadgets(me, t.Gadgets)
	end
    end
end

function DefUpdateExNoExtend(t)
    if t.Update == nil then
	t.UpdateEx = function(me)
	end
    else
	t.UpdateEx = t.Update
    end
end

function DefEnterExWithExtend(t, e)
    if t.Enter == nil then
	t.EnterEx = function(me)
	    ReplaceEnv(e.EnterEx)(me)
	    EnableGadgets(me, t.Gadgets)
	end
    else
	t.EnterEx = function(me)
	    ReplaceEnv(e.EnterEx)(me)
	    EnableGadgets(me, t.Gadgets)
	    ReplaceEnv(t.Enter)(me)
	end
    end
end

function DefLeaveExWithExtend(t, e)
    if t.Leave == nil then
	t.LeaveEx = function(me)
	    ReplaceEnv(e.LeaveEx)(me)
	    DisableGadgets(me, t.Gadgets)
	end
    else
	t.LeaveEx = function(me)
	    ReplaceEnv(e.LeaveEx)(me)
	    ReplaceEnv(t.Leave)(me)
	    DisableGadgets(me, t.Gadgets)
	end
    end
end

function DefUpdateExWithExtend(t, e)
    if t.Update == nil then
	t.UpdateEx = function(me)
	    return ReplaceEnv(e.UpdateEx)(me)
	end
    else
	t.UpdateEx = function(me)
	    local ret = ReplaceEnv(e.UpdateEx)(me)
	    if ret == nil then
		return ReplaceEnv(t.Update)(me)
	    else
		return ret
	    end
	end
    end
end

function DefState(this, t)
    local ext = t.Extends

    if ext == nil then
	DefEnterExNoExtend(t)
	DefUpdateExNoExtend(t)
	DefLeaveExNoExtend(t)
    else
	if _G.type(ext) ~= "table" then
	    _G.error("'Extends' must be a table", 2)
	end

	DefEnterExWithExtend(t, ext)
	DefUpdateExWithExtend(t, ext)
	DefLeaveExWithExtend(t, ext)

	if t.Name == nil then
	    t.Name = ext.Name
	end
    end

    if _G.type(t.Name) ~= "string" then
	_G.error("'Name' must be a string", 2)
    end

    if _G.rawget(this, t.Name) ~= nil then
	_G.error("State '" .. t.Name .. "' already defined", 2)
    end

    this[t.Name] = t
    t.FullName = this.Class .. "::" .. t.Name
end

DefState(This, {
    Name = "state_main"
})

function ReplaceEnv(f)
    local name, upvalueIdx

    for i = 1, _G.math.huge do
	name = _G.debug.getupvalue(f, i)
	if (name == "_ENV") then
	    upvalueIdx = i
	    break
	elseif (name == nil) then
	    return f
	end
    end

    local upvalue
    for i = 1, _G.math.huge do
        name, upvalue = _G.debug.getupvalue(ReplaceEnv, i)
        if (name == "_ENV") then
	    _G.debug.setupvalue(f, upvalueIdx, upvalue)
            break
        end
    end

    return f
end

EntryState = state_main
