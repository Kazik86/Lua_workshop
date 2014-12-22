#include "renderer.h"

#include <stdexcept>

eRenderer* eRenderer::iMe = 0;

eRenderer::eRenderer()
{
    if (iMe)
	throw std::runtime_error("eRenderer: multiple instances not allowed.");

    iMe = this;
}

eRenderer::~eRenderer()
{
    iMe = 0;
}
