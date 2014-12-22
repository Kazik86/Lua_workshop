#ifndef RENDERER_H
#define RENDERER_H

class eRenderer
{
public:
    eRenderer();
    ~eRenderer();

private:
    eRenderer(const eRenderer& aOther);
    eRenderer& operator=(const eRenderer& aOther);

private:
    static eRenderer* iMe;
};

#endif // RENDERER_H
