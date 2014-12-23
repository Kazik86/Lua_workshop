#ifndef RENDERER_H
#define RENDERER_H

#include <list>
#include <SDL2/SDL.h>
#include <string>

class eRenderable
{
public:
    eRenderable() {}
    virtual ~eRenderable() {}

    virtual void draw() = 0;
};

class eRenderer
{
public:
    eRenderer();
    ~eRenderer();

    static eRenderer* getMe() { return iMe; }

    void init(const std::string& aCaption, int aX, int aY, int aWidth, int aHeight, int aFlags);
    void render();

private:
    eRenderer(const eRenderer& aOther);
    eRenderer& operator=(const eRenderer& aOther);

private:
    static eRenderer* iMe;
    SDL_Window* iWindow;
    SDL_Renderer* iRenderer;
    std::list<eRenderable*> iRenderables;
};

#endif // RENDERER_H
