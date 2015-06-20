Class = "Actor"

function Init(me)
    --me.DumpState = 1
end

function Shift(me, newState)
    if newState == nil then
	_G.error("In state '" .. me.State.FullName .. "': 'Shift' to non existent state.")
    end

    local oldState = me.State
    local parentChanged = false
    local newStateParent = GetParent(newState)

    if oldState ~= nil then
	local oldStateParent = GetParent(oldState)
	parentChanged = oldStateParent ~= newStateParent 

	if oldStateParent ~= nil and parentChanged then
	    ReplaceEnv(oldStateParent.LeaveEx)(me)
	end

	ReplaceEnv(oldState.LeaveEx)(me)

	if (me.DumpState ~= nil) then
	    _G.print(Class .. " left " .. oldState.FullName)
	end
    end

    _G.eActor.shift(me.eActor, newState)
    me.State = newState

    if (me.DumpState ~= nil) then
	_G.print(Class .. " is about to enter " .. newState.FullName)
    end

    if newStateParent ~= nil and parentChanged then
	ReplaceEnv(newStateParent.EnterEx)(me)
    end

    ReplaceEnv(newState.EnterEx)(me)

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

-- EnterEx ------------------------------------------------------------

function GenEnterEx(state)
    return function(me)
	EnableGadgets(me, state.Gadgets)
    end
end

function GenEnterEx_enter(state)
    return function(me)
	EnableGadgets(me, state.Gadgets)
	ReplaceEnv(state.Enter)(me)
    end
end

function GenEnterEx_extends(state)
    return function(me)
	ReplaceEnv(GetExtended(state).EnterEx)(me)
	EnableGadgets(me, state.Gadgets)
    end
end

function GenEnterEx_extends_enter(state)
    return function(me)
	ReplaceEnv(GetExtended(state).EnterEx)(me)
	EnableGadgets(me, state.Gadgets)
	ReplaceEnv(state.Enter)(me)
    end
end

function DefEnterEx(state)
    if state.Extends == nil then
	if state.Enter == nil then
	    state.EnterEx = GenEnterEx(state)
	else
	    state.EnterEx = GenEnterEx_enter(state)
	end
    else
	if state.Enter == nil then
	    state.EnterEx = GenEnterEx_extends(state)
	else
	    state.EnterEx = GenEnterEx_extends_enter(state)
	end
    end
end

-- LeaveEx ------------------------------------------------------------

function GenLeaveEx(state)
    return function(me)
	DisableGadgets(me, state.Gadgets)
    end
end

function GenLeaveEx_leave(state)
    return function(me)
	ReplaceEnv(state.Leave)(me)
	DisableGadgets(me, state.Gadgets)
    end
end

function GenLeaveEx_extends(state)
    return function(me)
	ReplaceEnv(GetExtended(state).LeaveEx)(me)
	DisableGadgets(me, state.Gadgets)
    end
end

function GenLeaveEx_extends_leave(state)
    return function(me)
	ReplaceEnv(GetExtended(state).LeaveEx)(me)
	ReplaceEnv(state.Leave)(me)
	DisableGadgets(me, state.Gadgets)
    end
end

function DefLeaveEx(state)
    if state.Extends == nil then
	if state.Leave == nil then
	    state.LeaveEx = GenLeaveEx(state)
	else
	    state.LeaveEx = GenLeaveEx_leave(state)
	end
    else
	if state.Leave == nil then
	    state.LeaveEx = GenLeaveEx_extends(state)
	else
	    state.LeaveEx = GenLeaveEx_extends_leave(state)
	end
    end
end

-- UpdateEx -----------------------------------------------------------

function GenUpdateEx(state)
    return function(me)
    end
end

function GenUpdateEx_update(state)
    return state.Update
end

function GenUpdateEx_extends(state)
    return function(me)
	return ReplaceEnv(GetExtended(state).UpdateEx)(me)
    end
end

function GenUpdateEx_extends_update(state)
    return function(me)
	local ret = ReplaceEnv(GetExtended(state).UpdateEx)(me)
	if ret == nil then
	    return ReplaceEnv(state.Update)(me)
	else
	    return ret
	end
    end
end

function GenUpdateEx_parent(state)
    return function(me)
	return ReplaceEnv(GetParent(state).UpdateEx)(me)
    end
end

function GenUpdateEx_parent_update(state)
    return function(me)
	local ret = ReplaceEnv(GetParent(state).UpdateEx)(me)
	if ret == nil then
	    return ReplaceEnv(state.Update)(me)
	else
	    return ret
	end
    end
end

function DefUpdateEx(state)
    if state.Parent == nil then
	if state.Extends == nil then
	    if state.Update == nil then
		state.UpdateEx = GenUpdateEx(state)
	    else
		state.UpdateEx = GenUpdateEx_update(state)
	    end
	else
	    if state.Update == nil then
		state.UpdateEx = GenUpdateEx_extends(state)
	    else
		state.UpdateEx = GenUpdateEx_extends_update(state)
	    end
	end
    else
	if state.Extends == nil then
	    if state.Update == nil then
		state.UpdateEx = GenUpdateEx_parent(state)
	    else
		state.UpdateEx = GenUpdateEx_parent_update(state)
	    end
	else
	    -- state can not have 'Extends' and 'Parent' property at the same time.
	    -- Think about scenario when state being extended already has parent. We
	    -- would end up with two parents.
	    _G.error("Defining 'Extends' and 'Parent' simultaneously not allowed.", 3)
	end
    end
end

-----------------------------------------------------------------------

function GetParent(state)
    if state.DisableParentVirtualCall then
	return state.Parent
    else
	if state.Parent ~= nil then
	    return _ENV[state.Parent.Name]
	else
	    return nil
	end
    end
end

function GetExtended(state)
    if state.DisableExtendedVirtualCall then
	return state.Extends
    else
	if state.Extends ~= nil then
	    local e = _ENV[state.Extends.Name]
	    -- guard agains recursive call when state is in module at the
	    -- bottom of inheritance hierarchy
	    if e == state then
		return state.Extends
	    else
		return e
	    end
	else
	    return nil
	end
    end
end

function DefState(this, state)
    local parent = state.Parent
    local extends = state.Extends

    if parent ~= nil and _G.type(parent) ~= "table" then
	_G.error("'Parent' must be a table.", 2)
    end

    if extends ~= nil then
	if _G.type(extends) ~= "table" then
	    _G.error("'Extends' must be a table", 2)
	end
    end

    DefEnterEx(state)
    DefUpdateEx(state)
    DefLeaveEx(state)

    if extends ~= nil then
	if state.Name == nil then
	    state.Name = extends.Name
	end

	state.Parent = extends.Parent
    end

    if _G.type(state.Name) ~= "string" then
	_G.error("'Name' must be a string", 2)
    end

    -- warning instead of error because of real time update feature
    if _G.rawget(this, state.Name) ~= nil then
	_G.print("WARNING: State '" .. state.Name .. "' inside class '" .. this.Class .. "' already defined")
    end

    this[state.Name] = state
    state.FullName = this.Class .. "::" .. state.Name
    this[state.FullName] = state
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

    _G.debug.setupvalue(f, upvalueIdx, _ENV)

    return f
end

EntryState = state_main
