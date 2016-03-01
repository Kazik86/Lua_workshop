#include "gRandomPos.h"

#include "gTransform.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

DEFINE_GADGET_API(gRandomPos)
{
    {0, 0}
};

DEFINE_GADGET_CLASS(gRandomPos)

gRandomPos::gRandomPos():
    iTransform(0)
{
    iTransform = static_cast<gTransform*>(getActor()->findGadgetByClass("gTransform"));
    if (! iTransform) throw std::runtime_error(getActor()->getScript() + ", gRandomPos: 'gTransform' not found.");
}

gRandomPos::~gRandomPos()
{

}

int gRandomPos::update(lua_State* /* aLua */, float /* aDelta */)
{
    iTransform->setPos(glm::vec2(glm::linearRand(-50, 750), glm::linearRand(-600, -140)));
    return 0;
}
