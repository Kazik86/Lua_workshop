local Actor = {}

Actor.Class = "Base"
Actor.Derives = "tests.scripts.derivedLvl1"

function Actor.foo()
end

Actor.MetatableTests = {
    [1] = function ()
	      return Actor.onlyInDerivedLvl1 ~= nil
	  end,

    [2] = function ()
	      return Actor.onlyInDerivedLvl2 ~= nil
	  end
}

Actor.InheritanceTests = {
    [1] = function ()
	      return Actor.Derived1 ~= nil
	  end,

    [2] = function ()
	      return Actor.Derived1.foo ~= nil
	  end,

    [3] = function ()
	      return Actor.Derived1 == require(Actor.Derives)
	  end,

    [4] = function ()
	      return Actor.foo ~= Actor.Derived1.foo
	  end,

    [5] = function ()
	      return Actor.Derived2 ~= nil
	  end,

    [6] = function ()
	      return Actor.Derived2.foo ~= nil
	  end,

    [7] = function ()
	      return Actor.Derived1.foo ~= Actor.Derived2.foo
	  end,

    [8] = function ()
	      return Actor.Derived2.onlyInDerivedLvl2 ~= nil
	  end
}

function Actor.doTests(t)
    for i = 1, #t do
	if not t[i]() then
	    print("\tTest " .. i .. " failed.")
	    return false
	end
    end

    return true
end

function Actor.testMetatable()
    print "Testing metatable..."
    return Actor.doTests(Actor.MetatableTests)
end

function Actor.testInheritance()
    print "Testing inheritance..."
    return Actor.doTests(Actor.InheritanceTests)
end

return Actor
