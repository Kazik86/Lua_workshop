Class = "A"

function Foo(me)
    _G.error("Wrong function in virtual call.")
end

function Bar(me)
    Foo(me)
end
