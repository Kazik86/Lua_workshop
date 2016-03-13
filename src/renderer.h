#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

struct sRenderCopyEx
{
    SDL_Texture* iTexture;
    SDL_Rect iSrcRect;
    SDL_Rect iDstRect;
    float iAngle;
    SDL_Point iCenter;
    SDL_RendererFlip iFlip;
};

class eRenderable
{
    friend class eRenderer;

public:
    eRenderable(std::vector::size_type aNum);
    virtual ~eRenderable() {}

private:
    eRenderable(const eRenderable& aOther);
    eRenderable& operator=(const eRenderable& aOther);

public:
    virtual void draw() = 0;
    void addToRenderingQueue();
    void setZOrder(int aVal) { iZOrder = aVal; }

protected:
    int iZOrder;
    std::vector<sRenderCopyEx> iRCE;
};

class eRenderer
{
public:
    eRenderer();
    ~eRenderer();

    static eRenderer* getMe() { return iMe; }

    void init(const std::string& aCaption, int aX, int aY, int aWidth, int aHeight, int aFlags);
    void render(float aDelta);
    //SDL_Renderer* getRaw() { return iRenderer; }
    void addRenderable(eRenderable* aObj);
    void incRenderables();
    void clearRenderingQueue() { iRenderables.clear(); iRenderablesZOrder.clear(); }

    unsigned getFrameCntr() const { return iFrameCntr; }
    void resetFrameCntr() { iFrameCntr = 0; }

private:
    eRenderer(const eRenderer& aOther);
    eRenderer& operator=(const eRenderer& aOther);

private:
    void renderCopyEx(const std::vector<eRenderable*>& aCollection) const;

private:
    static eRenderer* iMe;
    SDL_Window* iWindow;
    SDL_Renderer* iRenderer;
    std::vector<eRenderable*> iRenderables;
    std::vector<eRenderable*> iRenderablesZOrder;
    decltype(iRenderables)::size_type iRenderablesNum;
    unsigned iFrameCntr;
};

#endif // RENDERER_H
