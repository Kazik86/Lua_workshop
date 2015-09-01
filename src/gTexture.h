#ifndef G_TEXTURE_H
#define G_TEXTURE_H

#include "gadget.h"
#include "renderer.h"

class gMove;
class gRotate;

class gTexture: public eGadget, public eRenderable
{
    DECLARE_GADGET_CLASS()

public:
    gTexture();
    ~gTexture();

    int  update(lua_State* aLua, float aDelta);
    void draw(SDL_Renderer* aRenderer, float aDelta);
    void begin();

    // props
    DECLARE_GADGET_ACCESSOR(getName, setName)
    DECLARE_GADGET_ACCESSOR(getSdlRectX, setSdlRectX)
    DECLARE_GADGET_ACCESSOR(getSdlRectY, setSdlRectY)
    DECLARE_GADGET_ACCESSOR(getSdlRectW, setSdlRectW)
    DECLARE_GADGET_ACCESSOR(getSdlRectH, setSdlRectH)
    DECLARE_GADGET_WRITER(setPosFromActor)
    DECLARE_GADGET_WRITER(setMoveGadget)
    DECLARE_GADGET_WRITER(setRotFromActor)
    DECLARE_GADGET_WRITER(setRotateGadget)

private:
    SDL_Texture* iTexture;
    std::string iName;
    SDL_Rect iSdlRect;
    bool iPosFromActor;
    bool iRotFromActor;
    gMove* i_gMove;
    gRotate* i_gRotate;
};

#endif // G_TEXTURE_H
