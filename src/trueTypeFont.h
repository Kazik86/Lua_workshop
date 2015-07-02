#ifndef TRUE_TYPE_FONT_H
#define TRUE_TYPE_FONT_H

#include "fontMgr.h"

#include <SDL2/SDL_ttf.h>

class eTrueTypeFont: public eFont
{
public:
    eTrueTypeFont(const sFontProps& aProps);
    ~eTrueTypeFont();

    void init();
    int  drawGlyph(SDL_Renderer* aRenderer, char aChar, int aX, int aY) const;

private:
    eTrueTypeFont(const eTrueTypeFont& aOther);
    eTrueTypeFont& operator=(const eTrueTypeFont& aOther);

    enum { KGlyphsNum = 177 };

    struct sGlyphMetrics
    {
        int iMinX;
        int iMaxX;
        int iMinY;
        int iMaxY;
        int iAdvance;
        int iTextureW;
        int iTextureH;
    };

    void initGlyphs();

private:
    TTF_Font* iFont;
    const sFontProps& iProps;
    SDL_Texture* iGlyphTextures[KGlyphsNum];
    sGlyphMetrics iGlyphMetrics[KGlyphsNum];
};


#endif // TRUE_TYPE_FONT_H
