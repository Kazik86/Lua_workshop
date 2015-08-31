Class = "Master"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function test(me)
    local a = _G.eActorMgr.add("tests/scripts/actorsApi/virtualCall/A.lua")
    local b = _G.eActorMgr.add("tests/scripts/actorsApi/virtualCall/B.lua")
    _G.A.fun(a)
    _G.B.fun(b)
    _G.assert(a.val == "actor A")
    _G.assert(b.val == "actor B")
end

