#ifndef __NOOP_GRAPHICS_ENGINE_H__
#define __NOOP_GRAPHICS_ENGINE_H__

#include "GraphicsEngine.h"

class NoopGraphicsEngine : public GraphicsEngine {
public:
    virtual ~NoopGraphicsEngine() {}

    virtual void setVideoMode(int width,
                              int height,
                              int bpp,
                              int flags,
                              bool fullscreen) 
	{}

    virtual void setPixel(unsigned int x,
                          unsigned int y,
                          const Color& c)
	{}

    virtual void update() {}
    virtual void beginDraw() {}

    virtual int getScreenHeight() { return 0; }
    virtual int getScreenWidth() { return 0; }

    virtual void drawImage(boost::any image, int x, int y) {}  
	virtual void drawTextSolid(const std::string& fontName,
							   int fontSize,
							   const std::string& text,
							   int x, int y,
							   int red, int green, int blue) {}
	virtual void setClipRect(int ulx, int uly, int width, int height) {}
	virtual void clearClipRect() {}
};

#endif	// __NOOP_GRAPHICS_ENGINE_H__
