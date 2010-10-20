#include "dm.h"

void
img_reader_init(char *image_mappings)
{
	int num_images;
	FILE *fin;
	int result;
	char name[512], extended_filename[512];
	SDL_Rect clip_rect;
	unsigned long flags;
	
	LOG("loading images from \"%s\"\n", image_mappings);	

	fin = fopen(image_mappings, "r");

	/* first parameter is the number of images, initilize graphics
	 * cache with this */
	result = fscanf(fin, "%d", &num_images);
	assert(result == 1);
		
	gfx_cache_destroy();
	gfx_cache_init(num_images);

	/* read each line and intern the image in the graphics cache */	
	while(1) {
		result = fscanf(fin, "%d \"%[^\"]\" %hd %hd %hd %hd %ld",
						&num_images, name, &clip_rect.x, &clip_rect.y,
						&clip_rect.w, &clip_rect.h, &flags);
		
		if(result == EOF) 
			break;
		else if(result != 7)
			/* error in the file */
			assert(0);

		result = snprintf(extended_filename, 512, "media/%s", name);
		assert(result >= 0);

		gfx_cache_intern(extended_filename, num_images, &clip_rect, flags);
	}

	fclose(fin);
}

void
img_reader_destroy()
{
	gfx_cache_destroy();
}
