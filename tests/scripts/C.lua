Class = "C"
Derives = "tests/scripts/B.lua"

function foo()
end

MetatableTests = {
    [1] = function ()
	      return onlyInB ~= nil
	  end,

    [2] = function ()
	      return onlyInC ~= nil
	  end
}

InheritanceTests = {
    [1] = function ()
	      return B ~= nil
	  end,

    [2] = function ()
	      return B.foo ~= nil
	  end,

    [3] = function ()
	      return B == require(Derives)
	  end,

    [4] = function ()
	      return foo ~= B.foo
	  end,

    [5] = function ()
	      return A ~= nil
	  end,

    [6] = function ()
	      return A.foo ~= nil
	  end,

    [7] = function ()
	      return B.foo ~= A.foo
	  end,

    [8] = function ()
	      return A.onlyInA ~= nil
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
