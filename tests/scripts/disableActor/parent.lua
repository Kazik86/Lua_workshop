Class = "Parent"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    me.child = CreateActor(me, "tests/scripts/disableActor/child.lua")
end

Super.DefState(This, {
    Name = "state_main",

    Update = function(me)
	Disable(me)
    end
})

Super.DefState(This, {
    Extends = Super.state_Disabled,

    Update = function(me)
	_G.assert(false)
    end
})

function test(me)
    _G.assert(me.State == state_Disabled)
    _G.assert(me.child.State == _G.Child.state_Disabled)
    Enable(me)
    _G.assert(me.State == state_main)
    _G.assert(me.child.State == _G.Child.state_main)
end

EntryState = state_main
