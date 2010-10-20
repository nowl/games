#ifndef __IMAGE_LOADER_H__
#define __IMAGE_LOADER_H__

#include <SDL.h>
#include <SDL_image.h>

int image_loader_load(char *alias, char *filename, int x, int y, int width, int height);
SDL_Surface *image_loader_get(char *alias);
void image_loader_cleanup();

#endif /* __IMAGE_LOADER_H__ */
