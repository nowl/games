#ifndef __RENDERABLE_INTERFACE__
#define __RENDERABLE_INTERFACE__

#include "SDLGraphicsContext.h"

class RenderableInterface {
public:
    virtual ~RenderableInterface() {}

    virtual void render(SDLGraphicsContext *gc, float interpolation) = 0;
};

#endif /* __RENDERABLE_INTERFACE__ */
