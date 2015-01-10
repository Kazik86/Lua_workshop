Class = "C"
Super = _G.eLuaModuleMgr.derive("tests/scripts/B.lua")

function foo()
end

function onlyInC()
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
	      return B == Super
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
	    _G.print("\tTest " .. i .. " failed.")
	    return false
	end
    end

    return true
end

function testMetatable()
    _G.print "Testing metatable..."
    return doTests(MetatableTests)
end

function testInheritance()
    _G.print "Testing inheritance..."
    return doTests(InheritanceTests)
end
