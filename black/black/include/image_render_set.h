#ifndef __IMAGE_RENDER_SET_H__
#define __IMAGE_RENDER_SET_H__

#include <SDL.h>

void image_render_set_create(char *name);
void image_render_set_add(char *name, char *image_name, int num_ticks);
SDL_Surface *image_render_set_get_image(char *name, int cur_tick);
void image_render_set_cleanup();

#endif /* __IMAGE_RENDER_SET_H__ */
