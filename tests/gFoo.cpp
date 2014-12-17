#include "gFoo.h"

namespace
{
    DEFINE_GADGET_PROPERTY(gFoo, Height, int)
}

DEFINE_GADGET_API(gFoo)
{
    REGISTER_GADGET_PROPERTY(Height),
    {0, 0}
};

DEFINE_GADGET_CLASS(gFoo)
