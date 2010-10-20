#include "dm.h"

static SDL_Surface** GRAPHICS_CACHE = NULL;
static unsigned int GRAPHICS_CACHE_SIZE = 0;
static char PREVIOUS_FILENAME[512];
static SDL_Surface *PREVIOUS_IMAGE;

void
gfx_cache_init(unsigned int max_num_images)
{
	unsigned int i;

	GRAPHICS_CACHE_SIZE = max_num_images;
	GRAPHICS_CACHE = malloc(sizeof(*GRAPHICS_CACHE) * GRAPHICS_CACHE_SIZE);

	for(i=0; i<GRAPHICS_CACHE_SIZE; i++)
		GRAPHICS_CACHE[i] = NULL;
}


void
gfx_cache_destroy()
{
	unsigned int i;
	for(i=0; i<GRAPHICS_CACHE_SIZE; i++)
		if(GRAPHICS_CACHE[i])
			SDL_FreeSurface(GRAPHICS_CACHE[i]);
	if(GRAPHICS_CACHE)
		free(GRAPHICS_CACHE);
	GRAPHICS_CACHE_SIZE = 0;

    if(PREVIOUS_IMAGE)
        SDL_FreeSurface(PREVIOUS_IMAGE);
}

void
gfx_cache_intern(char *filename, unsigned int image_num, SDL_Rect *clipping, unsigned long flags)
{
	if(clipping)
		LOG("loading image %d: \"%s\", clip_x = %d, clip_y = %d, width = %d, height = %d, flags = %ld\n",
			image_num, filename, clipping->x, clipping->y, clipping->w, clipping->h, flags);
	else
		LOG("loading image %d: \"%s\", flags = %ld\n", image_num, filename, flags);
	
	SDL_Surface *image;
    if(PREVIOUS_IMAGE && !strncmp(PREVIOUS_FILENAME, filename, 512) )
        image = PREVIOUS_IMAGE;
    else {
        image = IMG_Load(filename); 
		assert(image);
    
		strncpy(PREVIOUS_FILENAME, filename, 512);
        
        if(PREVIOUS_IMAGE)
            SDL_FreeSurface(PREVIOUS_IMAGE);

        PREVIOUS_IMAGE = image;
		
		SDL_SetAlpha(image, 0, 0);
    }

	if(clipping) {
		SDL_Surface* temp = SDL_CreateRGBSurface(image->flags,
												 clipping->w,
												 clipping->h,
												 image->format->BytesPerPixel * 8, 
												 image->format->Rmask,
												 image->format->Gmask,
												 image->format->Bmask,
												 image->format->Amask);
		int result = SDL_BlitSurface(image, clipping, temp, NULL);
		assert(result == 0);
   	
		/* swap in the new surface */
		image = temp;
	}
	
	/* process any flags */
	if(flags & GCF_HMIRROR)
		image = sdl_flip_surf_horiz(image);
	
	GRAPHICS_CACHE[image_num] = image;
}

SDL_Surface*
gfx_cache_get(unsigned int image_num)
{
	return GRAPHICS_CACHE[image_num];
}
