Class = "Actor"

function Shift(me, newState)
    _G.eActor.shift(me.eActor, newState)
end

function DefStageNoExt(t, s, se)
    if t[s] == nil then
	t[se] = function(me)
	end
    else
	t[se] = t[s]
    end
end

function DefStageWithExt(t, e, s, se)
    if t[s] == nil then
	t[se] = e[se]
    else
	t[se] = function(me)
	    e[se](me)
	    t[s](me)
	end
    end
end

function DefState(this, t)
    local ext = t.Extends

    if ext == nil then
	DefStageNoExt(t, "Enter", "EnterEx")
	DefStageNoExt(t, "Update", "UpdateEx")
	DefStageNoExt(t, "Leave", "LeaveEx")
    else
	if _G.type(ext) ~= "table" then
	    _G.error("'Extends' must be a table", 2)
	end

	DefStageWithExt(t, ext, "Enter", "EnterEx")
	DefStageWithExt(t, ext, "Update", "UpdateEx")
	DefStageWithExt(t, ext, "Leave", "LeaveEx")

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

EntryState = state_main
