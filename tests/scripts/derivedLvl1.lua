local Actor = {}

Actor.Class = "Derived1"
Actor.Derives = "tests.scripts.derivedLvl2"

function Actor.foo()
end

function Actor.onlyInDerivedLvl1()
end

return Actor
