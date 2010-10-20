#include <stdlib.h>
#include <SDL.h>

#include "black.h"

#define MAX_DIRTY_RECTS 512

unsigned int black_sdl_get_tick()
{
    return SDL_GetTicks();
}

void black_sdl_set_video_mode(sdl_graphics_context* gc,
                              unsigned int screen_width,
                              unsigned int screen_height,
                              unsigned char fullscreen)
{
    int flags = SDL_SWSURFACE|SDL_DOUBLEBUF;
    if(fullscreen)
        flags |= SDL_FULLSCREEN;
    gc->screen = SDL_SetVideoMode(screen_width, screen_height, 16, flags);
    if ( !gc->screen )
    {
        LOG("Unable to set video mode: %s\n", SDL_GetError());
        exit(1);
    }

    gc->dirty_rects = malloc(sizeof(*gc->dirty_rects) * MAX_DIRTY_RECTS);
    gc->dirty_rects_i = 0;
    gc->erase_rects = malloc(sizeof(*gc->erase_rects) * MAX_DIRTY_RECTS);
    gc->erase_rects_i = 0;

	gc->full_screen_update = 0;
}

void
black_sdl_fill_rect(game_manager *manager, int x, int y, int w, int h, int red, int green, int blue)
{
    SDL_Surface *screen = manager->sdl_driver->screen;

    SDL_Rect dest = {x, y, w, h};

    SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, red, green, blue));

    black_sdl_dirty_rect(manager, x, y, w, h);
}

void
black_sdl_dirty_rect(game_manager *manager, int x, int y, int w, int h)
{

    SDL_Rect *dirty_rects = manager->sdl_driver->dirty_rects;
    int dirty_rects_i = manager->sdl_driver->dirty_rects_i;

    /* do some manual clipping */
    if(x < 0) x = 0;
    if(x >= 800) x = 799;
    if(x+w >= 800) w = 799-x;
    if(y < 0) y = 0;
    if(y >= 600) y = 599;
    if(y+h >= 600) h = 599-y;

    if(dirty_rects_i < MAX_DIRTY_RECTS)
    {
        dirty_rects[dirty_rects_i].x = x;
        dirty_rects[dirty_rects_i].y = y;
        dirty_rects[dirty_rects_i].w = w;
        dirty_rects[dirty_rects_i].h = h;
		++manager->sdl_driver->dirty_rects_i;
    } else {
		manager->sdl_driver->full_screen_update = 1;
	}
}

void
black_sdl_draw_image(game_manager *manager, SDL_Surface *surf, int x, int y)
{
    SDL_Surface *screen = manager->sdl_driver->screen;

    SDL_Rect dest_rect = {x, y, 0, 0};
    SDL_BlitSurface(surf, NULL, screen, &dest_rect);

    black_sdl_dirty_rect(manager, x, y, surf->w, surf->h);
}

static
erase_rects(game_manager *manager)
{
    // erase screen

    SDL_Surface *screen = manager->sdl_driver->screen;
    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

    // copy erased rects

    memcpy(manager->sdl_driver->erase_rects,
		   manager->sdl_driver->dirty_rects,
		   manager->sdl_driver->dirty_rects_i * sizeof(*manager->sdl_driver->dirty_rects));
    manager->sdl_driver->erase_rects_i = manager->sdl_driver->dirty_rects_i;
}

void
black_sdl_flip(game_manager * manager)
{
    /* Dirty_rects holds the new image locations and erase_rects holds the previous frames
     * locations. Both should be updated and then dirty_rects should be copied over to erase_rects,
     * dirty_rects should then be erased.
     */

    SDL_Surface *screen = manager->sdl_driver->screen;

	if(!manager->sdl_driver->full_screen_update)
	{

		SDL_UpdateRects(screen, manager->sdl_driver->dirty_rects_i, manager->sdl_driver->dirty_rects);
		SDL_UpdateRects(screen, manager->sdl_driver->erase_rects_i, manager->sdl_driver->erase_rects);

		erase_rects(manager);

		manager->sdl_driver->dirty_rects_i = 0;
	} else {
        SDL_Flip(screen);
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

		manager->sdl_driver->full_screen_update = 0;
	}
}
