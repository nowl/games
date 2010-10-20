#ifndef SDL_H__
#define SDL_H__

void sdl_init();
void sdl_set_video_mode(int width, int height, unsigned char fullscreen);
unsigned long sdl_get_tick();
void sdl_close();

unsigned long sdl_get_color(unsigned char red,
                            unsigned char green,
                            unsigned char blue,
                            unsigned char alpha);
                            
void sdl_set_pixel(unsigned int x,
                   unsigned int y,
                   unsigned char red,
                   unsigned char green,
                   unsigned char blue,
                   unsigned char alpha);

void sdl_put_image(SDL_Surface *surf,
				   unsigned int x,
				   unsigned int y);

void sdl_fill_screen(SDL_Rect *fill_rect,
                     unsigned char red,
                     unsigned char green,
                     unsigned char blue);

#define SDL_BEGIN_DRAW                          \
    if(SDL_MUSTLOCK(SCREEN))                    \
        SDL_LockSurface(SCREEN);
#define SDL_END_DRAW                            \
    if(SDL_MUSTLOCK(SCREEN))                    \
        SDL_UnlockSurface(SCREEN);
                   

void sdl_update_screen(void);

SDL_Surface *sdl_flip_surf_horiz(const SDL_Surface* source);

#endif  /* SDL_H__ */
