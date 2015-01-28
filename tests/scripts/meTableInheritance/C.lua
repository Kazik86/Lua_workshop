Class = "C"
Super = _G.eLuaModuleMgr.derive("tests/scripts/meTableInheritance/B.lua")

function Init(me)
    me.c = 3
end

function testInC_1(me)
    _G.assert(me.a == 1)
    _G.assert(me.b == 2)
    _G.assert(me.c == 3)
end

function testInC_2(me)
    me.a = 10
    me.b = 20
    _G.assert(me.a == 10)
    _G.assert(me.A.a == 1)
    _G.assert(me.B.a == 1)
    _G.assert(me.B.b == 2)
    _G.assert(me.C.a == 10)
    _G.assert(me.C.b == 20)
    _G.assert(me.C.c == 3)
end

