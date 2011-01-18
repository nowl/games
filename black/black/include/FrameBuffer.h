#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

class FrameBuffer {
public:
    FrameBuffer(int width, int height);
    ~FrameBuffer();

    void bind();
    void unbind();
    GLuint getTexture()
    {
        return texture;
    }

private:
    GLuint fbo, texture;
};

#endif // __FRAMEBUFFER_H__
