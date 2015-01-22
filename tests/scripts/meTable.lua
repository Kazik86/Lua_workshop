Class = "Base"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function setAttribs1(me)
    me.foo = 1
    me.bar = 10
end

function setAttribs2(me)
    me.foo = 2
    me.bar = 20
end

function checkAttribs1(me)
    if me.foo ~= 1 or me.bar ~= 10 then
	error("checkAttribs1 test failed")
    end
end

function checkAttribs2(me)
    if me.foo ~= 2 or me.bar ~= 20 then
	error("checkAttribs2 test failed")
    end
end

Super.DefState(This, {
    Name = "state_main"
})

EntryState = state_main
