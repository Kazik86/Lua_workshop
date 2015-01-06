#include "gRandomPos.h"

#include "actor.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

DEFINE_GADGET_API(gRandomPos)
{
    {0, 0}
};

DEFINE_GADGET_CLASS(gRandomPos)

gRandomPos::gRandomPos()
{

}

gRandomPos::~gRandomPos()
{

}

void gRandomPos::update(lua_State* /* aLua */, float /* aDelta */)
{
    getActor()->setPos(glm::vec2(glm::linearRand(-50, 750), glm::linearRand(-600, -140)));
}
