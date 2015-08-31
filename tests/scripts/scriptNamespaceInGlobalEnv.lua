Class = "Foo"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function foo(me)
end

function test(me)
    _G.assert(_G.Foo.foo == foo)
end
