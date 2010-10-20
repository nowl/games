#ifndef __SDL_GRAPHICS_ENGINE_H__
#define __SDL_GRAPHICS_ENGINE_H__

#include <string>

#include "GraphicsEngine.h"
#include "SDLManager.h"

class SDLGraphicsEngine : public GraphicsEngine {
public:
    SDLGraphicsEngine(SDLManager *sdl)
        : mSDLMgr(*sdl), mDrawingSurface(NULL),
          mScreenHeight(0), mScreenWidth(0)
        {}

    virtual ~SDLGraphicsEngine() {}

    virtual void setVideoMode(int width,
                              int height,
                              int bpp,
                              int flags,
                              bool fullscreen);

    virtual void setPixel(unsigned int x,
                          unsigned int y,
                          const Color& c);

    virtual void update();
    virtual void beginDraw();

    virtual int getScreenHeight() { return mScreenHeight; }
    virtual int getScreenWidth() { return mScreenWidth; }

    void setDrawingSurface(SDL_Surface *s) { mDrawingSurface = s; }
  
    SDL_Surface *getScreen() { return mScreen; }

    virtual void drawImage(boost::any image, int x, int y);
	virtual void drawTextSolid(const std::string& fontName,
							   int fontSize,
							   const std::string& text,
							   int x, int y,
							   int red, int green, int blue);
	virtual void setClipRect(int ulx, int uly, int width, int height);
	virtual void clearClipRect();
  
private:
    inline unsigned long getColor(const Color& color);

    SDLManager &mSDLMgr;
    // TODO: should this be a const pointer?
    SDL_Surface * mScreen;
    SDL_Surface * mDrawingSurface;

    int mScreenHeight, mScreenWidth;

    class SurfaceLock {
    public:
        SurfaceLock(SDL_Surface *s) : mSurf(s) {
            if(SDL_MUSTLOCK(mSurf)) SDL_LockSurface(mSurf);
        }
        ~SurfaceLock() {
            if(SDL_MUSTLOCK(mSurf)) SDL_UnlockSurface(mSurf);
        }
    private:
        SDL_Surface *mSurf;
    };

};

#endif	// __SDL_GRAPHICS_ENGINE_H__
