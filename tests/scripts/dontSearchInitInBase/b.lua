Class = "Derived"
Super = _G.eLuaModuleMgr.derive("tests/scripts/dontSearchInitInBase/a.lua")

function test(me)
    _G.assert(me.base == 1)
end

