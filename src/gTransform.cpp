#include "gTransform.h"

namespace
{

int getPos(lua_State* aLua)
{
    const gTransform* me = Script::getGadget<gTransform>(aLua);
    glm::vec2 pos = me->getPos();
    Script::pushVal<decltype(pos.x)>(aLua, pos.x);
    Script::pushVal<decltype(pos.y)>(aLua, pos.y);
    return 2;
}

int setPos(lua_State* aLua)
{
    gTransform* me = Script::getGadget<gTransform>(aLua);
    glm::vec2 pos;
    pos.x = Script::getVal<decltype(pos.x)>(aLua, 2);
    pos.y = Script::getVal<decltype(pos.y)>(aLua, 3);
    me->setPos(pos);
    return 0;
}

}

DEFINE_GADGET_API(gTransform)
{
    // methods
    {"getPos", ::getPos},
    {"setPos", ::setPos},

    {0, 0}
};

DEFINE_GADGET_CLASS(gTransform)

gTransform::gTransform():
    iPos(0, 0),
    iRot(0),
    iScale(1.f)
{

}
