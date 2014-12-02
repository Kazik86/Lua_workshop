Class = "Base"
Derives = "tests/scripts/derivedLvl1.lua"

function foo()
end

MetatableTests = {
    [1] = function ()
	      return onlyInDerivedLvl1 ~= nil
	  end,

    [2] = function ()
	      return onlyInDerivedLvl2 ~= nil
	  end
}

InheritanceTests = {
    [1] = function ()
	      return Derived1 ~= nil
	  end,

    [2] = function ()
	      return Derived1.foo ~= nil
	  end,

    [3] = function ()
	      return Derived1 == require(Derives)
	  end,

    [4] = function ()
	      return foo ~= Derived1.foo
	  end,

    [5] = function ()
	      return Derived2 ~= nil
	  end,

    [6] = function ()
	      return Derived2.foo ~= nil
	  end,

    [7] = function ()
	      return Derived1.foo ~= Derived2.foo
	  end,

    [8] = function ()
	      return Derived2.onlyInDerivedLvl2 ~= nil
	  end
}

function doTests(t)
    for i = 1, #t do
	if not t[i]() then
	    print("\tTest " .. i .. " failed.")
	    return false
	end
    end

    return true
end

function testMetatable()
    print "Testing metatable..."
    return doTests(MetatableTests)
end

function testInheritance()
    print "Testing inheritance..."
    return doTests(InheritanceTests)
end
