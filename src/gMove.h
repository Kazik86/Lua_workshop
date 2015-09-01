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
    float iSpeed;
    float iMass;
    glm::vec2 iSpeedVec;
    float iFriction;
    glm::vec2  iForce;
};

#endif // G_MOVE_H
