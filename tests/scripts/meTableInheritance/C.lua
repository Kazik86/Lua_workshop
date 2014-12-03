Class = "C"
Derives = "tests/scripts/meTableInheritance/B.lua"

function OnInit(me)
    me.c = 3
end

function test1(me)
    if me.a ~= 1 or
       me.b ~= 2 or
       me.c ~= 3 then
       _G.error("MeTable inheritance test1 failed.")
   end
end

function test2(me)
    me.a = 10
    if me.a ~= 10 or me.A.a ~= 1 then
	_G.error("MeTable inheritance test2 failed.")
    end
end

function test3(me)
    onlyInB(me)
    local result = virtualFun(me)
    if me.c ~= result then
	_G.error("MeTable inheritance test3 failed.")
    end
end

function virtualFun(me)
    return me.c
end
