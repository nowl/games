#include "ParticleRenderer.h"

#include <stdlib.h>

ParticleRenderer::ParticleRenderer(float x, float y, float z, int numParticles)
    : texture("part.bmp")
{
    float mult = 0.5;

    for(int i=0; i<numParticles; i++)
    {
        float dy = 8*(float)rand() / RAND_MAX + 2;
        float dx = mult*((float)rand() / RAND_MAX) - mult/2;
        float dz = mult*((float)rand() / RAND_MAX) - mult/2;
        Particle p = {x, y, z, dx, dy, dz, -1, 0};
        particles.push_back(p);
    }
}

void ParticleRenderer::billboardStart()
{
    float modelview[16];
    int i,j;

    // save the current modelview matrix
    glPushMatrix();

    // get the current modelview matrix
    glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

    // undo all rotations
    // beware all scaling is lost as well
    for( i=0; i<3; i++ )
        for( j=0; j<3; j++ ) {
            if ( i==j )
                modelview[i*4+j] = 1.0;
            else
                modelview[i*4+j] = 0.0;
        }

    // set the modelview with no rotations and scaling
    glLoadMatrixf(modelview);
}

void ParticleRenderer::billboardEnd()
{
    // restores the modelview matrix
    glPopMatrix();
}

void
ParticleRenderer::Render(GraphicsContext &context, float interpolation, void *data)
{
    glShadeModel(GL_SMOOTH);

    //billboardStart();



    texture.bind();

    for(int i=0; i<particles.size(); i++)
    {
        Particle p = particles[i];
        float size = 5;

        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_QUADS);
        glTexCoord2f( 0, 0 );
        glVertex3f( p.x, p.y, p.z );
        glTexCoord2f( 1, 0 );
        glVertex3f( p.x+size, p.y, p.z );
        glTexCoord2f( 1, 1 );
        glVertex3f( p.x+size, p.y+size, p.z );
        glTexCoord2f( 0, 1 );
        glVertex3f( p.x, p.y+size, p.z );
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2f( 0, 0 );
        glVertex3f( p.x, p.y+size/2, p.z-size/2 );
        glTexCoord2f( 1, 0 );
        glVertex3f( p.x, p.y+size/2, p.z+size/2 );
        glTexCoord2f( 1, 1 );
        glVertex3f( p.x+size, p.y+size/2, p.z+size/2 );
        glTexCoord2f( 0, 1 );
        glVertex3f( p.x+size, p.y+size/2, p.z-size/2 );
        glEnd();

        glBegin(GL_QUADS);
        glTexCoord2f( 0, 0 );
        glVertex3f( p.x+size/2, p.y, p.z-size/2 );
        glTexCoord2f( 1, 0 );
        glVertex3f( p.x+size/2, p.y, p.z+size/2 );
        glTexCoord2f( 1, 1 );
        glVertex3f( p.x+size/2, p.y+size, p.z+size/2 );
        glTexCoord2f( 0, 1 );
        glVertex3f( p.x+size/2, p.y+size, p.z-size/2 );
        glEnd();
    }

    texture.unbind();

    //billboardEnd();
}

void
ParticleRenderer::update()
{
    for(int i=0; i<particles.size(); i++)
    {
        float t = (float)particles[i].time/10;
        particles[i].time++;
        particles[i].x += particles[i].dx;
        particles[i].z += particles[i].dz;
        particles[i].y = particles[i].dy * t + 0.5 * particles[i].gravity * t * t;

        if(particles[i].y < -20)
        {
            float mult = 0.5;

            particles[i].x = 0;
            particles[i].y = 0;
            particles[i].z = 0;
            particles[i].dx = mult*((float)rand() / RAND_MAX) - mult/2;
            particles[i].dy = 8*(float)rand() / RAND_MAX+2;
            particles[i].dz = mult*((float)rand() / RAND_MAX) - mult/2;
            particles[i].time = 0;
        }
    }
}
