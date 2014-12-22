Class = "A"
Derives = "scripts/Actor.lua"

function OnInit(me)
    shift(me, state_main)
end

function callVirtuallFun(me)
    return virtualFun(me)
end

function virtualFun(me)
    return nil
end

state_main = {
    enter = function(me)
    end,

    leave = function(me)
    end,

    update = function(me)
    end
}
