Class = "A"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function fun1(me)
    me.val = "actor A"
end

function fun(me, child)
    -- we start with _ENV == A
    if child ~= nil then
	-- but this call will set _ENV == B
	_G.B.fun(child, nil)
    end

    -- as a result the below will call 'B::fun1' for 'me' being A.lua
    fun1(me)
end

function test(me)
    local child = _G.eActorMgr.add("tests/scripts/actorsApi/replacingEnvWasABadIdea/B.lua")
    fun(me, child)
    _G.assert(me.val == "actor A")
    _G.assert(child.val == "actor B")
end
