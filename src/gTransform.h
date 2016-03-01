#ifndef G_TRANSFORM_H
#define G_TRANSFORM_H

#include "gadget.h"

#include <glm/glm.hpp>

class gTransform: public eGadget
{
    DECLARE_GADGET_CLASS()

public:
    gTransform();

public:
    const glm::vec2& getPos() const { return iPos; }
    void setPos(const glm::vec2& aPos) { iPos = aPos; }

    double getRotate() const { return iRot; }
    void setRotate(double aRot) { iRot = aRot; }

private:
    glm::vec2 iPos;
    double iRot;
    float iScale;
};


#endif // G_TRANSFORM_H
