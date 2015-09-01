#ifndef G_MOVE_H
#define G_MOVE_H

#include "gadget.h"

#include <glm/glm.hpp>

class gMove: public eGadget
{
    DECLARE_GADGET_CLASS()

public:
    gMove();
    ~gMove();

    int update(lua_State* aLua, float aDelta);
    const glm::vec2& getDir() const { return iDir; }
    float getSpeed() const { return iSpeed; }

    // props
    DECLARE_GADGET_WRITER(setDirX)
    DECLARE_GADGET_WRITER(setDirY)
    DECLARE_GADGET_WRITER(setSpeed)

private:
    glm::vec2 iDir;
    glm::vec2 iSpeedVec;
    glm::vec2 iForce;
    float iSpeed;
    float iMass;
    float iFriction;
    glm::vec2 friction();
};

#endif // G_MOVE_H
