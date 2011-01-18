#ifndef __PARTICLE_RENDERER_H__
#define __PARTICLE_RENDERER_H__

#include "Black.h"

#include <vector>

class ParticleRenderer : public IRenderable
{
public:
    ParticleRenderer(float x, float y, float z, int numParticles);

    virtual void Render(GraphicsContext &context, float interpolation, void *data);

    void update();

private:
    struct Particle {
        float x, y, z;
        float dx, dy, dz;
        float gravity;
        int time;
    };

    std::vector<Particle> particles;

    FileImageTexture texture;
    void billboardStart();
    void billboardEnd();
};

#endif // __PARTICLE_RENDERER_H__
