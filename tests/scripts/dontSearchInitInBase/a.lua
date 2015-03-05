Class = "Base"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function Init(me)
    if me.base == nil then
	me.base = 1
    else
	me.base = me.base + 1
    end
end
