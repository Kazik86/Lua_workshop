Class = "Actor"

function Init(me)
    me.Children = {}
    --me.DumpState = 1
end

function Shift(me, newStateRaw)
    if newStateRaw == nil then
	_G.error("In state '" .. me.State.FullName .. "': 'Shift' to non existent state.")
    end

    local newState = me.Env[newStateRaw.Name]

    local oldState = me.State
    local parentChanged = not oldState
    local newStateParent = GetParent(me, newState)

    if oldState ~= nil then
	local oldStateParent = GetParent(me, oldState)
	parentChanged = oldStateParent ~= newStateParent 

	if oldStateParent ~= nil and parentChanged then
	    oldStateParent.LeaveEx(me)
	end

	oldState.LeaveEx(me)

	if (me.DumpState ~= nil) then
	    _G.print(me.Env.Class .. " left " .. oldState.FullName)
	end
    end

    if (me.DumpState ~= nil) then
	_G.print(me.Env.Class .. " is about to enter " .. newState.FullName)
    end

    if newStateParent ~= nil and parentChanged then
	newStateParent.EnterEx(me)
    end

    newState.EnterEx(me)

    _G.eActor.shift(me.eActor, newState)
    me.State = newState

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
	state.Enter(me)
    end
end

function GenEnterEx_extends(state)
    return function(me)
	GetExtended(me, state).EnterEx(me)
	EnableGadgets(me, state.Gadgets)
    end
end

function GenEnterEx_extends_enter(state)
    return function(me)
	GetExtended(me, state).EnterEx(me)
	EnableGadgets(me, state.Gadgets)
	state.Enter(me)
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
	state.Leave(me)
	DisableGadgets(me, state.Gadgets)
    end
end

function GenLeaveEx_extends(state)
    return function(me)
	GetExtended(me, state).LeaveEx(me)
	DisableGadgets(me, state.Gadgets)
    end
end

function GenLeaveEx_extends_leave(state)
    return function(me)
	GetExtended(me, state).LeaveEx(me)
	state.Leave(me)
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
	return GetExtended(me, state).UpdateEx(me)
    end
end

function GenUpdateEx_extends_update(state)
    return function(me)
	local ret = GetExtended(me, state).UpdateEx(me)
	if ret == nil then
	    return state.Update(me)
	else
	    return ret
	end
    end
end

function GenUpdateEx_parent(state)
    return function(me)
	return GetParent(me, state).UpdateEx(me)
    end
end

function GenUpdateEx_parent_update(state)
    return function(me)
	local ret = GetParent(me, state).UpdateEx(me)
	if ret == nil then
	    return state.Update(me)
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

function GetParent(me, state)
    if state.DisableParentVirtualCall then
	return state.Parent
    else
	if state.Parent ~= nil then
	    return me.Env[state.Parent.Name]
	else
	    return nil
	end
    end
end

function GetExtended(me, state)
    if state.DisableExtendedVirtualCall then
	return state.Extends
    else
	if state.Extends ~= nil then
	    local e = me.Env[state.Extends.Name]
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

DefState(This, {
    Name = "state_Disabled",

    Enter = function(me)
	me.StateBeforeDisable = me.State
	_G.eActor.disable(me.eActor)
    end,

    Leave = function(me)
	_G.eActor.enable(me.eActor)
    end
})

function Disable(me)
    for i = 1, #me.Children do
	Disable(me.Children[i])
    end
    return Shift(me, state_Disabled)
end

function Enable(me)
    if me.StateBeforeDisable == nil then
	_G.print("WARNING: attempt to 'Enable' actor '" .. me.Env.Class .. "' defined in " .. me.Env.Script .. " without maching 'Disable'")
	return
    end

    Shift(me, me.StateBeforeDisable)
    for i = 1, #me.Children do
	Enable(me.Children[i])
    end
end

function CreateActor(me, script, props)
    local newActor = _G.eActorMgr.add(script, me.Id, props or {})
    me.Children[#me.Children + 1] = newActor
    return newActor
end

function Connect(sender, signal, recv, fun)
    if sender.Signals == nil then sender.Signals = {} end
    local s = sender.Signals

    if s.signal == nil then s.signal = {} end
    local t = s.signal

    t[#t + 1] = function() fun(recv) end
end

function Emit(me, signal)
    if me.Signals and me.Signals.signal then
	local s = me.Signals.signal
	for i = 1, #s do
	    s[i]()
	end
    end
end

EntryState = state_main
