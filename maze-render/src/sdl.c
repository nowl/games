#include "dm.h"

void
sdl_init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
}

void
sdl_set_video_mode(int width, int height, unsigned char fullscreen)
{
    SCREEN = SDL_SetVideoMode(width, 
                              height, 
                              32,
                              (fullscreen ? SDL_DOUBLEBUF | SDL_FULLSCREEN : SDL_DOUBLEBUF));
}

unsigned long
sdl_get_tick()
{
    return SDL_GetTicks();
}

void
sdl_close()
{
    IMG_Quit();
    SDL_Quit();
}

unsigned long
sdl_get_color(unsigned char red,
              unsigned char green,
              unsigned char blue,
              unsigned char alpha)
{
    if(alpha != 0)
        return SDL_MapRGB(SCREEN->format, red, green, blue);
    else
        return SDL_MapRGBA(SCREEN->format, red, green, blue, alpha);
}

void
sdl_set_pixel(unsigned int x,
              unsigned int y,
              unsigned char red,
              unsigned char green,
              unsigned char blue,
              unsigned char alpha)
{
    unsigned long color = sdl_get_color(red, green, blue, alpha);
    
    switch(SCREEN->format->BitsPerPixel) {
    case 32:
    {
        unsigned int index = y * SCREEN->pitch/4 + x;
      
        unsigned int *p = (unsigned int*)SCREEN->pixels + index;
      
        {
            SDL_BEGIN_DRAW;
            *p = color;
            SDL_END_DRAW;
        }
        break;
    }
    // TODO: The rest of these are untested
    case 24:
    {
        unsigned char index = y * SCREEN->pitch + x*3;
      
        unsigned int *p = (unsigned int *)((unsigned char*)SCREEN->pixels + index);
      
        {
            SDL_BEGIN_DRAW;
            *p = color;
            SDL_END_DRAW;
        }
        break;
    }
    case 16:
    {
        unsigned char index = y * SCREEN->pitch + x*2;
      
        unsigned short *p = (unsigned short *)((unsigned char*)SCREEN->pixels + index);
      
        {
            SDL_BEGIN_DRAW;
            *p = color;
            SDL_END_DRAW;
        }
        break;
    }
    case 8:
    {
        unsigned char index = y * SCREEN->pitch + x;
      
        unsigned char *p = (unsigned char*)SCREEN->pixels + index;
      
        {
            SDL_BEGIN_DRAW;
            *p = color;
            SDL_END_DRAW;
        }
        break;
    }
    default:
        break;
    }

}

void
sdl_put_image(SDL_Surface *surf,
			  unsigned int x,
			  unsigned int y)
{
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = surf->w;			/* necessary? */
	rect.h = surf->h;			/* necessary? */
	SDL_BlitSurface(surf, NULL, SCREEN, &rect);
}

void
sdl_fill_screen(SDL_Rect *fill_rect,
                unsigned char red,
                unsigned char green,
                unsigned char blue)
{
    int result = SDL_FillRect(SCREEN, fill_rect, SDL_MapRGB(SCREEN->format, red, green, blue));
    assert(result == 0);
}

void sdl_update_screen(void)
{
    SDL_Flip(SCREEN);
}

SDL_Surface *
sdl_flip_surf_horiz(const SDL_Surface* source)
{
	SDL_Surface* dest = SDL_CreateRGBSurface(source->flags,
											 source->w,
											 source->h,
											 source->format->BytesPerPixel * 8, 
											 source->format->Rmask,
											 source->format->Gmask,
											 source->format->Bmask,
											 source->format->Amask);

	unsigned char* src_pixels = (unsigned char*) source->pixels;
	unsigned char* dst_pixels = (unsigned char*) dest->pixels;
	unsigned int pitch = source->pitch;
	unsigned int bytes_pp = source->format->BytesPerPixel;
	unsigned int col, row, byte;

	SDL_LockSurface(dest);
	for(col = 0; col < pitch; col += bytes_pp) {
		for(row = 0; row < source->h; row++) {
			unsigned int dst_pos = row * pitch + col;
			unsigned int src_pos = row * pitch + (pitch - col - bytes_pp);
			
			/* copy the entire pixel */
			for(byte=0; byte < bytes_pp; byte++)
				dst_pixels[dst_pos+byte] = src_pixels[src_pos+byte];
		}
	}
	SDL_UnlockSurface(dest);
	
	return dest;
}
