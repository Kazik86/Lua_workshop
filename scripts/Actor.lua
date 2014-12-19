Class = "Actor"

function OnInit(me)
end

function shift(me, newState)
    _G.eActor.shift(me.eActor, newState)
end
