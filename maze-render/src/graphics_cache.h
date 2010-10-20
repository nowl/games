#ifndef GRAPHICS_CACHE_H__
#define GRAPHICS_CACHE_H__

void gfx_cache_init(unsigned int max_num_images);
void gfx_cache_destroy();

#define GCF_HMIRROR 1
void gfx_cache_intern(char *filename, unsigned int image_num, SDL_Rect *clipping, unsigned long flags);

SDL_Surface* gfx_cache_get(unsigned int image_num);


#endif	/* GRAPHICS_CACHE_H__ */
