Class = "Actor"

function OnInit(me)
end

function shift(me, newState)
    _G.eActor.shift(me.eActor, newState)
end

-- ta funkcja zgodnie z nazwą kopiuje stan "płytko", tzn. jeśli wewnatrz stanu
-- zdefiniowano jakieś tablice, to zostaną skopiowane tylko referencje do nich.
function shallowStateCopy(s)
  local new = {}
  for k, v in pairs(s) do
    new[k] = v
  end
  return new
end
