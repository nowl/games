#include "Black.h"

void Texture::bind()
{
    //glLoadIdentity();
    glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, texture );
}

void Texture::unbind()
{
    glDisable( GL_TEXTURE_2D );
}
