#include "trueTypeFont.h"

#include "renderer.h"

#include <stdexcept>

eTrueTypeFont::eTrueTypeFont(const sFontProps& aProps):
    iFont(0),
    iProps(aProps),
    iGlyphTextures()
{
    ::memset(iGlyphMetrics, 0, sizeof(iGlyphMetrics));
}

eTrueTypeFont::~eTrueTypeFont()
{
    for (size_t i = 0; i < KGlyphsNum; ++i)
        SDL_DestroyTexture(iGlyphTextures[i]);

    TTF_CloseFont(iFont);
}

void eTrueTypeFont::init()
{
    iFont = TTF_OpenFontRW(eFontMgr::getMe()->getTtfFontFile(iProps.iPath), 0, iProps.iSize);
    if(iFont == 0)
        throw std::runtime_error(std::string("eTrueTypeFont::init: TTF_OpenFontRW, ") + TTF_GetError());

    TTF_SetFontHinting(iFont, TTF_HINTING_NORMAL);
    TTF_SetFontStyle(iFont, TTF_STYLE_NORMAL);

    initGlyphs();
}

void eTrueTypeFont::initGlyphs()
{
    for (size_t i = 33; i < KGlyphsNum; ++i) {
        sGlyphMetrics& gm = iGlyphMetrics[i];
        Uint16 c = static_cast<Uint16>(i);

        TTF_GlyphMetrics(iFont, c, &gm.iMinX, &gm.iMaxX, &gm.iMinY, &gm.iMaxY, &gm.iAdvance);

        SDL_Surface* surface = TTF_RenderGlyph_Blended(iFont, c, iProps.iColor);
        if(surface == 0)
            throw std::runtime_error(std::string("eTrueTypeFont::initGlyphs: TTF_RenderGlyph_Blended, ") + TTF_GetError());

        SDL_Texture* tex = SDL_CreateTextureFromSurface(eRenderer::getMe()->getRaw(), surface);
        if(tex == 0)
            throw std::runtime_error(std::string("eTrueTypeFont::initGlyphs: SDL_CreateTextureFromSurface, ") + SDL_GetError());

        SDL_QueryTexture(tex, 0, 0, &gm.iTextureW, &gm.iTextureH);
        iGlyphTextures[i] = tex;
        SDL_FreeSurface(surface);
    }
}

int eTrueTypeFont::drawGlyph(SDL_Renderer* aRenderer, char aChar, int aX, int aY) const
{
    const sGlyphMetrics& gm = iGlyphMetrics[static_cast<size_t>(aChar)];
    SDL_Rect rect = {aX, aY, gm.iTextureW, gm.iTextureH};
    SDL_RenderCopy(aRenderer, iGlyphTextures[static_cast<size_t>(aChar)], 0, &rect);
    return gm.iTextureW;
}
