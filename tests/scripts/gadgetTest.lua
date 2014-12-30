Class = "Foo"
Derives = "scripts/Actor.lua"

function OnInit(me)
    me.gTexture = _G.gTexture.create(me)
    me.gTexture2 = _G.gTexture.create(me)
    me.gTexture:setName("foo")
    me.gTexture2:setName("bar")
    shift(me, state_main)
end

function test(me)
    _G.assert(_G.eActor.getGadgetsNum(me.eActor) == 2)
    _G.assert(me.gTexture:getName() == "foo")
    _G.assert(me.gTexture2:getName() == "bar")
end

state_main = {
    enter = function(me)
    end,

    leave = function(me)
    end,

    update = function(me)
    end
}
