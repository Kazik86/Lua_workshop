Class = "B"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function test(me)
    local a = CreateActor(me, "tests/scripts/actorsApi/A.lua")
    _G.A.fun(a)
    _G.assert(a.val == "actor A")
end

