Class = "Foo"

function test(me)
    me.gFoo = _G.gFoo.create()
    me.gFoo:setHeight(5)
    _G.assert(me.gFoo:getHeight() == 5)
    _G.print(me.gFoo:getHeight())
end
