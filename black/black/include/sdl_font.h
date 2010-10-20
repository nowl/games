#ifndef __SDL_FONT_H__
#define __SDL_FONT_H__

#include <SDL.h>

SDL_Surface * sdl_font_get_surf(char *font_name, int pt_size, char *text, int r, int g, int b);
int sdl_font_get_size(char *font_name, int pt_size, char *text, int *w, int *h);
void black_sdl_free_font_surfaces();
void sdl_font_cleanup();

#endif /* __SDL_FONT_H__ */
