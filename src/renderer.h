#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

class eRenderable
{
public:
    eRenderable();
    virtual ~eRenderable() {}

    virtual void draw(SDL_Renderer* aRenderer, float aDelta) = 0;
    void addToRenderingQueue();
};

class eRenderer
{
public:
    eRenderer();
    ~eRenderer();

    static eRenderer* getMe() { return iMe; }

    void init(const std::string& aCaption, int aX, int aY, int aWidth, int aHeight, int aFlags);
    void render(float aDelta);
    SDL_Renderer* getRaw() { return iRenderer; }
    void addRenderable(eRenderable* aObj);
    void incRenderables();
    void clearRenderingQueue() { iRenderables.clear(); }

private:
    eRenderer(const eRenderer& aOther);
    eRenderer& operator=(const eRenderer& aOther);

private:
    static eRenderer* iMe;
    SDL_Window* iWindow;
    SDL_Renderer* iRenderer;
    std::vector<eRenderable*> iRenderables;
    decltype(iRenderables)::size_type iRenderablesNum;
};

#endif // RENDERER_H
