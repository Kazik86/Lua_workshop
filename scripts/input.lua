-- based on https://wiki.libsdl.org/SDLScancodeLookup
local keys = { up = 82, down = 81, left = 80, right = 79 }

local function isKeyDown(key)
    return _G.eInput.isKeyDown(key)
end

local function up()
    return isKeyDown(keys.up)
end

local function down()
    return isKeyDown(keys.down)
end

local function left()
    return isKeyDown(keys.left)
end

local function right()
    return isKeyDown(keys.right)
end

return {
    keys = keys,
    up = up,
    down = down,
    left = left,
    right = right
}
