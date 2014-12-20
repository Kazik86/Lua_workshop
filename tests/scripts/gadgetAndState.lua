Class = "Foo"
Derives = "scripts/Actor.lua"

function OnInit(me)
    me.gFoo = _G.gFoo.create()
    shift(me, state_main)
end

state_main = {
    enter = function(me)
	me.gFoo:enable()
	me.gFoo:setVal(0)
    end,

    update = function(me)
    end,

    leave = function(me)
    end
}

function test(me)
    _G.assert(me.gFoo:getVal() == 2)
end
