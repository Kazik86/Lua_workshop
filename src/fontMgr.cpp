#include "fontMgr.h"

#include "trueTypeFont.h"

#include <stdexcept>

namespace
{
    const sFontProps KFontProps[eFontMgr::EFontsNum] = {
        { eFontMgr::ETtfSmall, eFontMgr::ETrueType, "fonts/DejaVuSansMono.ttf", 12, {255, 255, 255, 255} },
        { eFontMgr::ETtfMedium, eFontMgr::ETrueType, "fonts/DejaVuSansMono.ttf", 24, {255, 255, 255, 255} },
        { eFontMgr::ETtfBig, eFontMgr::ETrueType, "fonts/DejaVuSansMono.ttf", 42, {255, 255, 255, 255} },
        // { eFontMgr::EBitmap, eFontMgr::EBitmap, "fonts/font.png" /* other fields will be value-initialized */ }
    };
}

eFontMgr* eFontMgr::iMe = 0;

eFontMgr::eFontMgr()
{
    if (iMe)
        throw std::runtime_error("eFontMgr: multiple instances not allowed.");

    iMe = this;

    ::memset(iFonts, 0, sizeof(iFonts));
}

eFontMgr::~eFontMgr()
{
    for (int i = 0; i < EFontsNum; ++i) {
        delete iFonts[i];
        iFonts[i] = 0;
    }

    for (auto& f : iTtfFontFiles)
        SDL_RWclose(f.second);

    TTF_Quit();
    iMe = 0;
}

void eFontMgr::init()
{
    initTtf();

    for (int i = 0; i < EFontsNum; ++i) {
        const sFontProps& fp = KFontProps[i];

        if (fp.iId != i)
            throw std::logic_error("eFontMgr: fonts order in KFontProps doesn't match with eFontMgr::TFontId.");

        if (fp.iType == ETrueType)
            iFonts[i] = new eTrueTypeFont(fp);
        // else if (fp.iType == EBitmap)
        //     iFonts[i] = new eBitmapFont(fp.iPath);

        iFonts[i]->init();
    }
}

void eFontMgr::initTtf()
{
    if (TTF_Init() == -1) {
        std::string msg = "eFontMgr::initTtf, SDL_ttf: ";
        msg += TTF_GetError();
        throw std::runtime_error(msg);
    }
}

SDL_RWops* eFontMgr::getTtfFontFile(const std::string& aPath)
{
    SDL_RWops*& f = iTtfFontFiles[aPath];

    if (f == 0) {
        f = SDL_RWFromFile(aPath.c_str(), "r");
        if (f == 0)
            throw std::runtime_error(std::string("eFontMgr::getTtfFontFile: SDL_RWFromFile, ") + SDL_GetError());
    }

    return f;
}
