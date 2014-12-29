#ifndef G_TEXTURE_H
#define G_TEXTURE_H

#include "gadget.h"
#include "renderer.h"

class gTexture: public eGadget, public eRenderable
{
    DECLARE_GADGET_CLASS()

public:
    gTexture();
    ~gTexture();

    void update(lua_State* aLua, float aDelta);
    void draw(SDL_Renderer* aRenderer);
    void begin();

    // props
    DECLARE_GADGET_PROPERTY(Name, std::string)
    DECLARE_GADGET_PROPERTY(SdlRectX, int)
    DECLARE_GADGET_PROPERTY(SdlRectY, int)
    DECLARE_GADGET_PROPERTY(SdlRectW, int)
    DECLARE_GADGET_PROPERTY(SdlRectH, int)

private:
    SDL_Texture* iTexture;
};

#endif // G_TEXTURE_H
