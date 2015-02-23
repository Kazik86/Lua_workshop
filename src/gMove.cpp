#include "gMove.h"

DEFINE_GADGET_WRITER(gMove, setDirX, iDir.x)
DEFINE_GADGET_WRITER(gMove, setDirY, iDir.y)
DEFINE_GADGET_WRITER(gMove, setSpeed, iSpeed)

DEFINE_GADGET_API(gMove)
{
    REGISTER_GADGET_WRITER(setDirX)
    REGISTER_GADGET_WRITER(setDirY)
    REGISTER_GADGET_WRITER(setSpeed)
    {0, 0}
};

DEFINE_GADGET_CLASS(gMove)

gMove::gMove():
    iDir(0, 1),
    iSpeed(5)
{

}

gMove::~gMove()
{

}

void gMove::update(lua_State* /* aLua */, float aDelta)
{
    eActor* actor = getActor();
    const auto& oldPos = actor->getPos();
    actor->setPos(oldPos + iDir * iSpeed * aDelta);
}
