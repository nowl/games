#include "black.h"

/* quick cache will contain the previously loaded "raw" image */
static char *quick_cache_filename = NULL;
static SDL_Surface *quick_cache = NULL;

struct image_cache {
    /* reference to the surface object returned from loading a surface through SDL_image */
    SDL_Surface *surface;

    /* a copy of a string representing the alias of this image */
    char *alias;

    /* the structure used to include this in an rb
     * tree for easy retreival */
    RB_ENTRY(image_cache) _rb_tree_entry;
};

static RB_HEAD(image_cache_tree, image_cache) image_cache = RB_INITIALIZER(image_cache);

static int __cmp(struct image_cache *a, struct image_cache *b) {
    return strcmp(a->alias, b->alias);
}

RB_GENERATE_STATIC(image_cache_tree, image_cache, _rb_tree_entry, __cmp);

int
image_loader_load(char *alias,
                  char *filename,
                  int x,
                  int y,
                  int width,
                  int height)
{
    SDL_Surface *raw;

    /* check quick cache */
    if(!quick_cache_filename || strcmp(filename, quick_cache_filename) != 0) {
        raw = IMG_Load(filename);
        if(!raw)
        {
            LOG("IMG_Load: %s\n", IMG_GetError());
            return FAIL;
        }

        /* free if necessary */
        if(quick_cache_filename)
            free(quick_cache_filename);
        if(quick_cache)
            SDL_FreeSurface(quick_cache);

        quick_cache = raw;
        quick_cache_filename = strdup(filename);
        LOG("quick caching \"%s\"\n", filename);
    } else {
        raw = quick_cache;
    }

    SDL_Surface *image = SDL_CreateRGBSurface(0, width, height,
                                              raw->format->BitsPerPixel,
                                              raw->format->Rmask,
                                              raw->format->Gmask,
                                              raw->format->Bmask,
                                              raw->format->Amask);
    /* ignore alpha info */
    SDL_SetAlpha(image, 0, 0);
    /* XXX: set color key to 0 to mask out the transparent parts*/
    SDL_SetColorKey(image, SDL_SRCCOLORKEY, 0);

    if(!image)
    {
        SDL_FreeSurface(raw);
        free(quick_cache_filename);
        quick_cache = quick_cache_filename = NULL;
        LOG("SDL_CreateRGBSurface fail\n");
        return FAIL;
    }

    SDL_Rect source_rect = {x, y, width, height};

    int r = SDL_BlitSurface(raw, &source_rect, image, NULL);
    if(r != 0)
    {
        SDL_FreeSurface(image);
        SDL_FreeSurface(raw);
        free(quick_cache_filename);
        quick_cache = quick_cache_filename = NULL;
        LOG("SDL_BlitSurface fail: %s\n", SDL_GetError());
        return FAIL;
    }

    struct image_cache *new_entry = malloc(sizeof(*new_entry));
    new_entry->surface = image;
    new_entry->alias = strdup(alias);

    image_cache_tree_RB_INSERT(&image_cache, new_entry);

    LOG("cached cropped surface with alias \"%s\"\n", alias);

    return SUCCESS;
}

SDL_Surface *
image_loader_get(char *alias)
{
    struct image_cache tmp;
    tmp.alias = alias;
    struct image_cache *result;

    result = image_cache_tree_RB_FIND(&image_cache, &tmp);
    LOG("looking for alias %s --> got %x\n", alias, result);
    if(result)
        return result->surface;
    return NULL;
}

void
image_loader_cleanup()
{
    for(;;) {
        struct image_cache *entry = RB_ROOT(&image_cache);

        if(!entry)
            break;

        SDL_FreeSurface(entry->surface);
        free(entry->alias);

        entry = image_cache_tree_RB_REMOVE(&image_cache, entry);
        free(entry);
    }
}
