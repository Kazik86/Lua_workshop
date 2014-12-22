Class = "Foo"
Derives = "scripts/Actor.lua"

function OnInit(me)
    me.gFoo = _G.gFoo.create()
    me.gFoo2 = _G.gFoo.create()
    me.gFoo:setVal(5)
    me.gFoo2:setVal(3)
    shift(me, state_main)
end

function test(me)
    _G.assert(_G.eActor.getGadgetsNum(me.eActor) == 2)
    _G.assert(me.gFoo:getVal() == 5)
    _G.assert(me.gFoo2:getVal() == 3)
end

state_main = {
    enter = function(me)
    end,

    leave = function(me)
    end,

    update = function(me)
    end
}
