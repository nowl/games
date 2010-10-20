#include "black.h"

struct sound_cache {
    Mix_Chunk *sound;

    /* a copy of a string representing the alias of this sound */
    char *alias;

    /* the structure used to include this in an rb
     * tree for easy retreival */
    RB_ENTRY(sound_cache) _rb_tree_entry;
};

static RB_HEAD(sound_cache_tree, sound_cache) sound_cache = RB_INITIALIZER(sound_cache);

static int __cmp(struct sound_cache *a, struct sound_cache *b) {
    return strcmp(a->alias, b->alias);
}

RB_GENERATE_STATIC(sound_cache_tree, sound_cache, _rb_tree_entry, __cmp);

int
sdl_sound_load(char *file, char *alias)
{
    struct sound_cache *new_entry = malloc(sizeof(*new_entry));
    new_entry->sound = Mix_LoadWAV(file);
    new_entry->alias = strdup(alias);
    
    sound_cache_tree_RB_INSERT(&sound_cache, new_entry);

    LOG("cached sound from file \"%s\"\n", file);
    
    return SUCCESS;
}

void
sdl_sound_play(char *alias)
{
    struct sound_cache tmp;
    tmp.alias = alias;
    struct sound_cache *result;

    result = sound_cache_tree_RB_FIND(&sound_cache, &tmp);
    if(result)
        Mix_PlayChannel(-1, result->sound, 0);
}


void
sound_loader_cleanup()
{
    for(;;) {
        struct sound_cache *entry = RB_ROOT(&sound_cache);

        if(!entry)
            break;

        Mix_FreeMusic(entry->sound);
        free(entry->alias);

        entry = sound_cache_tree_RB_REMOVE(&sound_cache, entry);
        free(entry);
    }
}
