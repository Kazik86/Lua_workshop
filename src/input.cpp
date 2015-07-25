#include "input.h"

#include "luaState.h"

#include <stdexcept>

eInput* eInput::iMe = 0;

namespace
{
    int isKeyDown(lua_State* aLua)
    {
        eInput* me = eInput::getMe();
        lua_pushboolean(aLua, me->isKeyDown(static_cast<SDL_Scancode>(luaL_checkint(aLua, 1))));
        return 1;
    }
}

DEFINE_USERDATA_API(eInput)
{
    {"isKeyDown", ::isKeyDown},
    {0, 0}
};

DEFINE_USERDATA_CLASS(eInput)

eInput::eInput()
{
    if (iMe)
	throw std::runtime_error("eInput: multiple instances not allowed.");

    iMe = this;
    iKeyboardState = SDL_GetKeyboardState(0);
}

eInput::~eInput()
{
    iMe = 0;
}
