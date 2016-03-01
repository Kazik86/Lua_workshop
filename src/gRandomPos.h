#ifndef G_RANDOM_POS_H
#define G_RANDOM_POS_H

#include "gadget.h"

class gTransform;

class gRandomPos: public eGadget
{
    DECLARE_GADGET_CLASS()

public:
    gRandomPos();
    ~gRandomPos();

    int update(lua_State* aLua, float aDelta);

private:
    gTransform* iTransform;
};

#endif // G_RANDOM_POS_H
