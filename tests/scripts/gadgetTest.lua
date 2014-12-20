Class = "Foo"

function OnInit(me)
    me.gFoo = _G.gFoo.create()
    me.gFoo2 = _G.gFoo.create()
    me.gFoo:setVal(5)
    me.gFoo2:setVal(3)
end

function test(me)
    _G.assert(_G.eActor.getGadgetsNum(me.eActor) == 2)
    _G.assert(me.gFoo:getVal() == 5)
    _G.assert(me.gFoo2:getVal() == 3)
end
