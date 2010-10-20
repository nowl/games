#include "SDLGraphicsEngine.h"
#include "FontCache.h"

using namespace std;

void 
SDLGraphicsEngine::setVideoMode(int width,
                                int height,
                                int bpp,
                                int flags,
                                bool fullscreen)
{
    unsigned long realFlags = 0;

    if(flags == DEFAULTS)
        realFlags = SDL_DOUBLEBUF | SDL_SWSURFACE;
    if(fullscreen)
        realFlags |= SDL_FULLSCREEN;

    mScreen = SDL_SetVideoMode(width, height, bpp, realFlags);

    // TODO: assume NULL is a bad return value
    if(!mScreen)
        throw DefException("set_video_mode failed");

    mDrawingSurface = mScreen;

    mScreenHeight = height;
    mScreenWidth = width;
}

void
SDLGraphicsEngine::setClipRect(int ulx,
							   int uly, 
							   int width,
							   int height)
{
	SDL_Rect rect;
	rect.x = ulx;
	rect.y = uly;
	rect.w = width;
	rect.h = height;
	SDL_SetClipRect(mScreen, &rect);
}

void
SDLGraphicsEngine::clearClipRect()
{
	SDL_SetClipRect(mScreen, NULL);
}

unsigned long
SDLGraphicsEngine::getColor(const Color& color)
{
    unsigned char r = color.getRedAsInt();
    unsigned char g = color.getGreenAsInt();
    unsigned char b = color.getBlueAsInt();
    unsigned char a = color.getAlphaAsInt();

    if(a != 0)
        return SDL_MapRGB(mScreen->format, r, g, b);
    else
        return SDL_MapRGBA(mScreen->format, r, g, b, a);
}

void
SDLGraphicsEngine::setPixel(unsigned int x,
                            unsigned int y,
                            const Color& c)
{
    unsigned long color = getColor(c);
  
    switch(mDrawingSurface->format->BitsPerPixel) {
    case 32:
    {
        unsigned int index = y * mDrawingSurface->pitch/4 + x;
      
        unsigned int *p = (unsigned int*)mDrawingSurface->pixels + index;
      
        {
            SurfaceLock sl(mDrawingSurface);
	
            *p = color;
        }
        break;
    }
    // TODO: The rest of these are untested
    case 24:
    {
        unsigned char index = y * mDrawingSurface->pitch + x*3;
      
        unsigned int *p = (unsigned int *)((unsigned char*)mDrawingSurface->pixels + index);
      
        {
            SurfaceLock sl(mDrawingSurface);
	
            *p = color;
        }
        break;
    }
    case 16:
    {
        unsigned char index = y * mDrawingSurface->pitch + x*2;
      
        unsigned short *p = (unsigned short *)((unsigned char*)mDrawingSurface->pixels + index);
      
        {
            SurfaceLock sl(mDrawingSurface);
	
            *p = color;
        }
        break;
    }
    case 8:
    {
        unsigned char index = y * mDrawingSurface->pitch + x;
      
        unsigned char *p = (unsigned char*)mDrawingSurface->pixels + index;
      
        {
            SurfaceLock sl(mDrawingSurface);
	
            *p = color;
        }
        break;
    }
    default:
        break;
    }
}

void SDLGraphicsEngine::update() {
    SDL_Flip(mScreen);
}

void SDLGraphicsEngine::beginDraw() {
    setDrawingSurface(getScreen());
    //SDL_FillRect(mDrawingSurface, NULL, 0);
}

void SDLGraphicsEngine::drawImage(boost::any image, 
                                  int x,
                                  int y)
{
    SDL_Surface *buf = boost::any_cast<SDL_Surface *>(image);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = buf->w;
    rect.h = buf->h;
    SDL_BlitSurface(buf, NULL, getScreen(), &rect); 
}

void SDLGraphicsEngine::drawTextSolid(const string& fontName,
									  int fontSize,
									  const string& text,
									  int x, int y,
									  int red, int green, int blue)
{
    SDL_Color color;
	color.r = red;
	color.g = green;
	color.b = blue;

    SDL_Surface *text_surface;

	TTF_Font *font = GlobalFontCache.get(fontName, fontSize);

    text_surface = TTF_RenderText_Solid(font, text.c_str(), color);

    if(!text_surface)
        throw DefException(string("problem rendering font") + TTF_GetError());

	SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = text_surface->w;
    rect.h = text_surface->h;

    if (text_surface != NULL)
    {
        SDL_BlitSurface(text_surface, NULL, getScreen(), &rect);
        SDL_FreeSurface(text_surface);
    }
    else
    {
		throw DefException("problem rendering font");
    }
}
