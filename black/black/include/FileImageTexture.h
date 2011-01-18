#ifndef __FILE_IMAGE_TEXTURE_H__
#define __FILE_IMAGE_TEXTURE_H__

#include "Texture.h"

class FileImageTexture : public Texture
{
public:
    FileImageTexture(const char *filename);
};

#endif // __FILE_IMAGE_TEXTURE_H__
