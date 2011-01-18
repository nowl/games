#ifndef __TEXTURE_H__
#define __TEXTURE_H__

class Texture
{
public:
    virtual ~Texture()
    {
        glDeleteTextures( 1, &texture );
    }

    void bind();
    void unbind();

protected:
    GLuint texture;
};

#endif // __TEXTURE_H__
