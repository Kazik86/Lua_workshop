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

stateMain = {
    enter = function(me)
	me.mainEnterVisited = true
    end,

    update = function(me)
	me.mainUpdateVisited = true
	shift(me, stateFoo)
    end,

    leave = function(me)
	me.mainLeaveVisited = true
    end
}

stateFoo = {
    enter = function(me)
	me.fooEnterVisited = true
    end,

    update = function(me)
	me.fooUpdateVisited = true
	shift(me, stateMain)
    end,

    leave = function(me)
	me.fooLeaveVisited = true
    end
}

function test(me)
    _G.assert(me.mainEnterVisited)
    _G.assert(me.mainUpdateVisited)
    _G.assert(me.mainLeaveVisited)

    _G.assert(me.fooEnterVisited)
    _G.assert(me.fooUpdateVisited)
    _G.assert(me.fooLeaveVisited)
end

EntryState = stateMain
