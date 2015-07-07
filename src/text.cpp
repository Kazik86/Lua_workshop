#include "text.h"

eText::eText(int aX, int aY, eFontMgr::TFontId aFont):
    iBuffer(),
    iX(aX),
    iY(aY),
    iFont(aFont)
{
}

void eText::draw(SDL_Renderer* aRenderer)
{
    const eFont* f = eFontMgr::getMe()->getFont(iFont);
    int x = iX;

    for (size_t i = 0; iBuffer[i] != '\0'; ++i)
        x += f->drawGlyph(aRenderer, iBuffer[i], x, iY);
}

void eText::format(const char* aFmt, ...)
{
    ::memset(iBuffer, 0, sizeof(iBuffer));
    va_list argList;
    
    va_start(argList, aFmt);
    vsnprintf(iBuffer, sizeof(iBuffer), aFmt, argList);
    va_end(argList);
}
