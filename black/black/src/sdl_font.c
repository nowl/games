#include "black.h"

static struct
{
    char *name;
    int ptsize;
    TTF_Font *font;
} *font_cache = NULL;
static int font_cache_len = 0;
static int font_cache_cap = 0;

SDL_Surface **font_surfs = NULL;
static int font_surfs_len = 0;
static int font_surfs_cap = 0;

static TTF_Font *
get_font(char *font_name, int pt_size)
{
    int found = FALSE;
    int index = 0;

    /* linear search for font */
    int i;
    for(i=0; i<font_cache_len; i++)
    {
        if(font_cache[i].ptsize == pt_size && strcmp(font_cache[i].name, font_name) == 0)
        {
            index = i;
            found = TRUE;
            break;
        }
    }

    if(!found)
    {
        index = font_cache_len++;
        font_cache = memory_grow_to_size(font_cache, sizeof(*font_cache), &font_cache_cap, font_cache_len);

        /* make new entry */
        font_cache[index].font = TTF_OpenFont(font_name, pt_size);

        /* gracefully handle font error */
        if(!font_cache[index].font)
        {
            --font_cache_len;
            ERROR("problem loading font %s:%d\n", font_name, pt_size);
            return NULL;
        }

        font_cache[index].ptsize = pt_size;
        font_cache[index].name = strdup(font_name);
    }

    return font_cache[index].font;
}

static void
cache_surf(SDL_Surface *surf)
{
    ++font_surfs_len;
    font_surfs = memory_grow_to_size(font_surfs, sizeof(*font_surfs), &font_surfs_cap, font_surfs_len);
    font_surfs[font_surfs_len-1] = surf;
}

SDL_Surface *
sdl_font_get_surf(char *font_name, int pt_size, char *text, int r, int g, int b)
{
    TTF_Font * font = get_font(font_name, pt_size);
    if(!font)
        return NULL;

    SDL_Color color = {r, g, b};

    SDL_Color bg = {0, 0, 0};
    SDL_Surface *surf = TTF_RenderText_Shaded(font, text, color, bg);

    /* ignore alpha info */
    SDL_SetAlpha(surf, 0, 0);
    /* XXX: set color key to 0 to mask out the transparent parts*/
    SDL_SetColorKey(surf, SDL_SRCCOLORKEY, 0);

    cache_surf(surf);

    return surf;
}

int
sdl_font_get_size(char *font_name, int pt_size, char *text, int *w, int *h)
{
    TTF_Font * font = get_font(font_name, pt_size);
    if(!font)
        return NULL;

    return TTF_SizeText(font, text, w, h);
}

void
black_sdl_free_font_surfaces()
{
    int i;
    for(i=0; i<font_surfs_len; i++)
    {
        SDL_FreeSurface(font_surfs[i]);
    }
    font_surfs_len = 0;
}

void
sdl_font_cleanup()
{
    int i;

    for(i=0; i<font_cache_len; i++)
    {
        free(font_cache[i].name);
        TTF_CloseFont(font_cache[i].font);
    }

    free(font_cache);
    font_cache = NULL;
    font_cache_len = 0;
    font_cache_cap = 0;

    free(font_surfs);
    font_surfs = NULL;
    font_surfs_len = 0;
    font_surfs_cap = 0;
}
