Class = "Derived1"

function Foo(me)
    error("Wrong function in virtual call.")
end

function Bar(me)
    Foo(me)
end
