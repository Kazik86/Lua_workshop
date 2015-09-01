#include "gRotate.h"

DEFINE_GADGET_WRITER(gRotate, setDir, iDir)
DEFINE_GADGET_WRITER(gRotate, setOmega, iOmega)

DEFINE_GADGET_API(gRotate)
{
    REGISTER_GADGET_WRITER(setDir)
    REGISTER_GADGET_WRITER(setOmega)
    {0, 0}
};

DEFINE_GADGET_CLASS(gRotate)

gRotate::gRotate():
    iDir(1),
    iOmega(5)
{
}

gRotate::~gRotate()
{
}

int gRotate::update(lua_State* /* aLua */, float aDelta)
{
    eActor* actor = getActor();
    const auto& oldRot = actor->getRotate();
    actor->setRotate(oldRot + iDir * iOmega * aDelta);

    return 0;
}
