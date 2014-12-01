local Actor = {}

Actor.Class = "Derived1"

function Actor.Foo(me)
    print "Foo from Derived1"
end

function Actor.Bar(me)
    Actor.Foo(me)
end

return Actor
