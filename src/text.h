#ifndef TEXT_H
#define TEXT_H

#include "fontMgr.h"
#include "renderer.h"

class eText: public eRenderable
{
public:
    eText(int aX, int aY, eFontMgr::TFontId aFont);

    void draw(SDL_Renderer* aRenderer);
    void format(const char* aFmt, ...);

private:
    eText(const eText& aOther);
    eText& operator=(const eText& aOther);

private:
    char iBuffer[256];
    int iX;
    int iY;
    eFontMgr::TFontId iFont;
};

#endif // TEXT_H
