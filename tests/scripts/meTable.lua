local Actor = {}

Actor.Class = "Base"

function Actor.setAttribs1(me)
    me.foo = 1
    me.bar = 10
end

function Actor.setAttribs2(me)
    me.foo = 2
    me.bar = 20
end

function Actor.checkAttribs1(me)
    if me.foo ~= 1 or me.bar ~= 10 then
	error("checkAttribs1 test failed")
    end
end

function Actor.checkAttribs2(me)
    if me.foo ~= 2 or me.bar ~= 20 then
	error("checkAttribs2 test failed")
    end
end

return Actor
