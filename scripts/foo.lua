-- zrobić z poniższego szablon Aktora;
-- dodac szablon stanu
local Actor = {}

Actor.Class = "Foo"
Actor.Derives = "scripts.bar"

function Actor.properties(me)
    me.lives = 5
end

function Actor.gadgets(me)
    me.gFoo = gFoo.cretae()
    me.button1 = eActor.new("scripts.button")
end

function Actor.init()
    eFsm.setDefaultState("foo")
    me.button1.set{ width = 10, height = 20 }
end

function Actor.onSomeEvent(me)
    -- do smth
end

Actor.state_foo = {
}

return Actor
