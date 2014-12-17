#ifndef G_FOO_H
#define G_FOO_H

#include "gadget.h"

class gFoo: public eGadget
{
    DECLARE_GADGET_CLASS()
    DECLARE_GADGET_PROPERTY(Height, int)
};

#endif // G_FOO_H
