#include "gMove.h"
#include <iostream>

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
  iSpeedVec(0,0),
  iForce(0,100),
  iMass(5),
  iFriction(0.1)
{

}

gMove::~gMove()
{

}

int gMove::update(lua_State* /* aLua */, float aDelta)
{
    eActor* actor = getActor();
    const auto& oldPos = actor->getPos();
    iSpeedVec += (iForce/iMass)*aDelta + friction();
    actor->setPos(oldPos + iSpeedVec);
    //std::cout << "x:" << (iForce/iMass).x << ", y:" << (iForce/iMass).y << ", friction:" << friction().y << std::endl;

    return 0;
}

glm::vec2 gMove::friction()
{
  glm::vec2 ret = {0,0};
  if (glm::length(iSpeedVec) != 0)
    ret = glm::normalize(iSpeedVec)*iFriction*glm::length(iSpeedVec)*glm::length(iSpeedVec)*(-1.0f);
  std::cout << "friction: " << ret.y << ", speed: " << iSpeedVec.y << std::endl;
  return ret;
}
