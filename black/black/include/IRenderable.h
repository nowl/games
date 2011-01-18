#ifndef __IRENDERABLE_H__
#define __IRENDERABLE_H__

class IRenderable
{
public:
    virtual ~IRenderable() {}

    virtual void Render(GraphicsContext &context, float interpolation, void *data) = 0;
};

#endif // __IRENDERABLE_H__
