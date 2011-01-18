#include "Black.h"

FrameBuffer::FrameBuffer(int width, int height)
{
    glGenFramebuffersEXT(1, &fbo);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    unbind();
}

FrameBuffer::~FrameBuffer()
{
    // TODO: delete texture and framebuffer
}

void
FrameBuffer::bind()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
}

void
FrameBuffer::unbind()
{
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
