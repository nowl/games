#ifndef __GRAPHICS_ENGINE_H__
#define __GRAPHICS_ENGINE_H__

#include <boost/any.hpp>
#include <boost/noncopyable.hpp>

#include <string>

#include "Geometry.h"
#include "Color.h"

class GraphicsEngine : private boost::noncopyable {
public:
    enum {DEFAULTS};
    
    GraphicsEngine() {};
    
    virtual ~GraphicsEngine() {};
    
    virtual void setVideoMode(int width,
                              int height,
                              int bpp,
                              int flags,
                              bool fullscreen) = 0;
    
    virtual void setPixel(unsigned int x,
                          unsigned int y,
                          const Color& c) = 0;
    
    virtual int getScreenHeight() = 0;
    virtual int getScreenWidth() = 0;
    
    virtual void update() = 0;
    virtual void beginDraw() = 0;
   
    virtual void drawImage(boost::any image, int x, int y) = 0;
    virtual void drawTextSolid(const std::string& fontName,
							   int fontSize,
							   const std::string& text,
							   int x, int y,
							   int red, int green, int blue) = 0;
	virtual void setClipRect(int ulx, int uly, int width, int height) = 0;
	virtual void clearClipRect() = 0;
};

#endif	// __GRAPHICS_ENGINE_H__
