Class = "B"
Derives = "tests/scripts/meTableInheritance/A.lua"

function OnInit(me)
    me.b = 2
end

function onlyInB(me)
    virtualFun(me)
end

function virtualFun(me)
end
