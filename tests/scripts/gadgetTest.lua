Class = "Foo"

function OnInit(me)
    me.gFoo = _G.gFoo.create()
    me.gFoo2 = _G.gFoo.create()
    me.gFoo:setHeight(5)
    me.gFoo2:setHeight(3)
end

function test(me)
    _G.assert(_G.eActor.getGadgetsNum(me.eActor) == 2)
    _G.assert(me.gFoo:getHeight() == 5)
    _G.assert(me.gFoo2:getHeight() == 3)
end
