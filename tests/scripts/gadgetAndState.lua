Class = "Foo"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function OnInit(me)
    me.gTexture = _G.gTexture.create(me)
    me.gTexture:setName("foo")
end

state_main = {
    enter = function(me)
	me.gTexture:enable()
    end,

    update = function(me)
    end,

    leave = function(me)
    end
}

function test(me)
    _G.assert(me.gTexture:getName() == "foo")
end

EntryState = state_main
