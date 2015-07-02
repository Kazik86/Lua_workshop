#ifndef FONT_MGR_H
#define FONT_MGR_H

#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

class eFont
{
public:
    eFont() {}
    virtual ~eFont() {}
    virtual int drawGlyph(SDL_Renderer* aRenderer, char aChar, int aX, int aY) const = 0;
    virtual void init() = 0;

private:
    eFont(const eFont& aOther);
    eFont& operator=(const eFont& aOther);
};

class eFontMgr
{
public:
    enum TFontId { ETtfSmall = 0, ETtfMedium, ETtfBig, EFontsNum };
    enum TFontType { EBitmap, ETrueType };

    eFontMgr();
    ~eFontMgr();

    static eFontMgr* getMe() { return iMe; }
    void init();
    const eFont* getFont(TFontId aFont) const { return iFonts[aFont]; }

private:
    eFontMgr(const eFontMgr& aOther);
    eFontMgr& operator=(const eFontMgr& aOther);

private:
    static eFontMgr* iMe;
    eFont* iFonts[EFontsNum];


    // true type fonts stuff
public:
    SDL_RWops* getTtfFontFile(const std::string& aPath);

private:
    void initTtf();

private:
    std::unordered_map<std::string, SDL_RWops*> iTtfFontFiles;
};

struct sFontProps
{
    eFontMgr::TFontId iId;
    eFontMgr::TFontType  iType;
    std::string iPath;
    int iSize;
    SDL_Color iColor;
};

#endif // FONT_MGR_H
