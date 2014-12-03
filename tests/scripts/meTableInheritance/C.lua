Class = "C"
Derives = "tests/scripts/meTableInheritance/B.lua"

function OnInit(me)
    me.c = 3
end

function test1(me)
    _G.assert(me.a == 1)
    _G.assert(me.b == 2)
    _G.assert(me.c == 3)
end

function test2(me)
    me.a = 10
    _G.assert(me.a == 10)
    _G.assert(me.A.a == 1)
end

