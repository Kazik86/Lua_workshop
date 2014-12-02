-- zrobić z poniższego szablon Aktora;
-- dodac szablon stanu
Class = "Foo"
Derives = "scripts/bar.lua"

function properties(me)
    me.lives = 5
end

function gadgets(me)
    me.gFoo = gFoo.cretae()
    me.button1 = eActor.new("scripts.button")
end

function init()
    eFsm.setDefaultState("foo")
    me.button1.set{ width = 10, height = 20 }
end

function onSomeEvent(me)
    -- do smth
end

state_foo = {
}
