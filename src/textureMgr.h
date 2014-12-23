#ifndef TEXTURE_MGR_H
#define TEXTURE_MGR_H

#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

class eTextureMgr
{
public:
    eTextureMgr();
    ~eTextureMgr();

    static eTextureMgr* getMe() { return iMe; }
    void init();
    SDL_Texture* getTexture(const std::string& aName);

private:
    eTextureMgr(const eTextureMgr& aOther);
    eTextureMgr& operator=(const eTextureMgr& aOther);

private:
    static eTextureMgr* iMe;
    std::unordered_map<std::string, SDL_Texture*> iTextures;
};

#endif // TEXTURE_MGR_H
