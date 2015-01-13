Class = "Actor"

function OnInit(me)
end

function shift(me, newState)
    _G.eActor.shift(me.eActor, newState)
end

state_main = {
    enter = function(me)
    end,

    leave = function(me)
    end,

    update = function(me)
    end
}

EntryState = state_main
