#include "gRotate.h"

#include "gTransform.h"

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
    iOmega(5),
    iTransform(0)
{
    iTransform = static_cast<gTransform*>(getActor()->findGadgetByClass("gTransform"));
    if (! iTransform) throw std::runtime_error(getActor()->getScript() + ", gRotate: 'gTransform' not found.");
}

gRotate::~gRotate()
{
}

int gRotate::update(lua_State* /* aLua */, float aDelta)
{
    const auto& oldRot = iTransform->getRotate();
    iTransform->setRotate(oldRot + iDir * iOmega * aDelta);

    return 0;
}
