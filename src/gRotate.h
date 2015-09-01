#ifndef G_ROTATE_H
#define G_ROTATE_H

#include "gadget.h"

class gRotate: public eGadget
{
    DECLARE_GADGET_CLASS()

public:
    gRotate();
    ~gRotate();

    int update(lua_State* aLua, float aDelta);
    int getDir() const { return iDir; }
    float getOmega() const { return iOmega; }

    // props
    DECLARE_GADGET_WRITER(setDir)
    DECLARE_GADGET_WRITER(setOmega)

private:
    int iDir;
    float iOmega;
};

#endif // G_ROTATE_H
