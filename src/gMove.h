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

    void update(lua_State* aLua, float aDelta);

    // props
    DECLARE_GADGET_WRITER(setDirX)
    DECLARE_GADGET_WRITER(setDirY)
    DECLARE_GADGET_WRITER(setSpeed)

private:
    glm::vec2 iDir;
    float iSpeed;
};

#endif // G_MOVE_H
