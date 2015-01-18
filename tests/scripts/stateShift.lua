Class = "Foo"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function OnInit(me)
    me.mainEnterVisited = false
    me.mainUpdateVisited = false
    me.mainLeaveVisited = false

    me.fooEnterVisited = false
    me.fooUpdateVisited = false
    me.fooLeaveVisited = false
end

Super.DefState(This, {
    Name = "stateMain",

    Enter = function(me)
	me.mainEnterVisited = true
    end,

    Update = function(me)
	me.mainUpdateVisited = true
	return Shift(me, stateFoo)
    end,

    Leave = function(me)
	me.mainLeaveVisited = true
    end
})

Super.DefState(This, {
    Name = "stateFoo",

    Enter = function(me)
	me.fooEnterVisited = true
    end,

    Update = function(me)
	me.fooUpdateVisited = true
	return Shift(me, stateMain)
    end,

    Leave = function(me)
	me.fooLeaveVisited = true
    end
})

function test(me)
    _G.assert(me.mainEnterVisited)
    _G.assert(me.mainUpdateVisited)
    _G.assert(me.mainLeaveVisited)

    _G.assert(me.fooEnterVisited)
    _G.assert(me.fooUpdateVisited)
    _G.assert(me.fooLeaveVisited)
end

EntryState = stateMain
