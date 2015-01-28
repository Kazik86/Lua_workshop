Class = "B"
Super = _G.eLuaModuleMgr.derive("tests/scripts/meTableInheritance/A.lua")

function Init(me)
    me.b = 2
end

function testInB_1(me)
    _G.assert(me.a == 1)
    _G.assert(me.A.a == 1)
    _G.assert(me.b == 2)
    _G.assert(me.B.a == 1)
    _G.assert(me.B.b == 2)
end

function testInB_2(me)
    _G.assert(me.a == 10)
    _G.assert(me.A.a == 1)
    _G.assert(me.b == 20)
    _G.assert(me.B.a == 1)
    _G.assert(me.B.b == 2)
end
