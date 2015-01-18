Class = "Foo"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function OnInit(me)
    me.gTexture = _G.gTexture.create(me)
    me.gTexture:setName("foo")
end

Super.DefState(This, {
    Name = "state_main",

    Enter = function(me)
	me.gTexture:enable()
    end
})

function test(me)
    _G.assert(me.gTexture:getName() == "foo")
end

EntryState = state_main
