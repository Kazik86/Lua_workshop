local Actor = {}

Actor.Class = "Base"
Actor.Derives = "tests.scripts.virtualFunctions.derivedLvl1"

function Actor.Foo(me)
    print "Foo from Base"
end

return Actor
