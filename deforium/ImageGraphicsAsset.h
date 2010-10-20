#ifndef __IMAGE_GRAPHICS_ASSET_H__
#define __IMAGE_GRAPHICS_ASSET_H__

#include <SDL_image.h>

#include "GraphicsAsset.h"
#include "DefException.h"

class ImageGraphicsAsset : public GraphicsAsset {
public:
    virtual ~ImageGraphicsAsset() {
        SDL_FreeSurface(mSurf);
    }

    ImageGraphicsAsset(const std::string& name)
        : GraphicsAsset()
    {
        mSurf = IMG_Load(name.c_str());
        if(!mSurf)
            throw DefException(std::string("problem loading file: ") + name);

        mName = name;
    }

    virtual boost::any getContents() {
        return mSurf;
    }

private:
   SDL_Surface *mSurf;
};

#endif	// __IMAGE_GRAPHICS_ASSET_H__
