Class = "A"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function OnInit(me)
    shift(me, state_main)
end

function callVirtuallFun(me)
    return virtualFun(me)
end

function virtualFun(me)
    return 666
end

function test(me)
    _G.assert(666 == callVirtuallFun(me))
end

state_main = {
    enter = function(me)
    end,

    leave = function(me)
    end,

    update = function(me)
    end
}
