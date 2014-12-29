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
    DECLARE_GADGET_PROPERTY(Name)
    DECLARE_GADGET_PROPERTY(SdlRectX)
    DECLARE_GADGET_PROPERTY(SdlRectY)
    DECLARE_GADGET_PROPERTY(SdlRectW)
    DECLARE_GADGET_PROPERTY(SdlRectH)

private:
    SDL_Texture* iTexture;
    std::string iName;
    SDL_Rect iSdlRect;
};

#endif // G_TEXTURE_H
