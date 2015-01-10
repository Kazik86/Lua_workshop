Class = "B"
Super = _G.eLuaModuleMgr.derive("tests/scripts/virtualFunctions/A.lua")

function OnInit(me)
    me.c = 3
end

function test(me)
    _G.assert(me.c == callVirtuallFun(me))
end

function virtualFun(me)
    return me.c
end
