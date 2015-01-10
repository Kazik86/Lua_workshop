Class = "Foo"
Super = _G.eLuaModuleMgr.derive("scripts/Actor.lua")

function OnInit(me)
    shift(me, state_main)
end

function setActorFoo(me)
    ActorFooPtr = me.eActor
    ActorFooScript = _G.eActor.getScript(me.eActor)
end

function setActorBar(me)
    ActorBarPtr = me.eActor
    ActorBarScript = _G.eActor.getScript(me.eActor)
end

function test(me)
    _G.assert(ActorFooScript == "tests/scripts/scriptSupport.lua")
    _G.assert(ActorFooScript == ActorBarScript)
    _G.assert(ActorFooPtr ~= nil)
    _G.assert(ActorBarPtr ~= nil)
    _G.assert(ActorFooPtr ~= ActorBarPtr)
end

state_main = {
    enter = function(me)
    end,

    leave = function(me)
    end,

    update = function(me)
    end
}
