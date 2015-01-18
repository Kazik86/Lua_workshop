Class = "A"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function OnInit(me)
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

Super.DefState(This, {
    Name = "state_main"
})

EntryState = state_main
