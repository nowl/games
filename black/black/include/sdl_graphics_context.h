#ifndef __SDLGRAPHICSCONTEXT_H__
#define __SDLGRAPHICSCONTEXT_H__

#include <SDL.h>

struct sdl_graphics_context {
    SDL_Surface* screen;

    SDL_Rect *dirty_rects;
    int dirty_rects_i;
    SDL_Rect *erase_rects;
    int erase_rects_i;

	int full_screen_update; 	/* flag that determines whether to use dirty rects or not */
};

unsigned int black_sdl_get_tick();
void black_sdl_set_video_mode(sdl_graphics_context* gc,
                              unsigned int screen_width,
                              unsigned int screen_height,
                              unsigned char fullscreen);

struct game_manager;

void black_sdl_fill_rect(game_manager *manager, int x, int y, int w, int h, int red, int green, int blue);
void black_sdl_dirty_rect(game_manager *manager, int x, int y, int w, int h);
void black_sdl_draw_image(game_manager *manager, SDL_Surface *surf, int x, int y);
void black_sdl_flip(game_manager * manager);

#endif /* __SDLGRAPHICSCONTEXT_H__ */
