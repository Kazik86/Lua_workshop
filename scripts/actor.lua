local Actor = {}

function Actor.new(base)
    super = require(base)
end

function Actor.introduce()
    print "I'm actor!"
end

return Actor
