#include "black.h"

struct keyframe {
    SDL_Surface *image;
    unsigned int num_ticks;
};

struct image_render_set
{
    char *name;                    /* the name of this image set */
    struct keyframe **frames;      /* the array of keyframes */
    unsigned int num_frames;       /* the actual number of frames */
    unsigned int cap_frames;       /* the maximum number of frames allocated */

    RB_ENTRY(image_render_set) _rb_tree_entry;
};

static RB_HEAD(image_render_set_tree, image_render_set) image_render_sets = RB_INITIALIZER(image_render_sets);

static int set_cmp(struct image_render_set *a, struct image_render_set *b) {
    return strcmp(a->name, b->name);
}

RB_GENERATE_STATIC(image_render_set_tree, image_render_set, _rb_tree_entry, set_cmp);

void
image_render_set_create(char *name)
{
    struct image_render_set *new_entry = malloc(sizeof(*new_entry));
    new_entry->name = strdup(name);
    new_entry->num_frames = 0;
    new_entry->cap_frames = 0;
    new_entry->frames = NULL;

    image_render_set_tree_RB_INSERT(&image_render_sets, new_entry);
}

static struct image_render_set *
get_set(char *name)
{
    struct image_render_set t;
    t.name = name;

    /* attempt to retrieve from rb tree */
    return image_render_set_tree_RB_FIND(&image_render_sets, &t);
}

/* TODO: do we need a function to reset and image set? */

void
image_render_set_add(char *name, char *image_name, int num_ticks)
{
    int i;
    struct image_render_set *e = get_set(name);
    e->frames = memory_grow_to_size(e->frames,
                                    sizeof(*e->frames),
                                    &e->cap_frames,
                                    e->num_frames + 1);

    SDL_Surface *surface = image_loader_get(image_name);
    struct keyframe *frame = malloc(sizeof(*frame));
    frame->image = surface;
    frame->num_ticks = num_ticks;
    LOG("assigning surface at address %x\n", surface);
    e->frames[e->num_frames++] = frame;
    LOG("adding %d frames of %s to %s\n", num_ticks, image_name, name);
}

void
image_render_set_cleanup()
{
    for(;;) {
        struct image_render_set *entry = RB_ROOT(&image_render_sets);

        if(!entry)
            break;

        int i;
        for(i=0; i<entry->num_frames; i++)
            free(entry->frames[i]);

        free(entry->frames);
        free(entry->name);

        entry = image_render_set_tree_RB_REMOVE(&image_render_sets, entry);
        free(entry);
    }
}

SDL_Surface *
image_render_set_get_image(char *name, int cur_tick)
{
    struct image_render_set *e = get_set(name);
    SDL_Surface *result;

    /* hack to skip loop if only one frame */
    if(e->num_frames > 1)
    {
        int frame = 0;
        int skip_frames = 0;

        while(cur_tick >= 0)
        {
            cur_tick -= e->frames[frame]->num_ticks;
            skip_frames++;
        }

        --skip_frames;

        frame = (frame + skip_frames) % e->num_frames;
        
        result = e->frames[frame]->image;
    } else {
        result = e->frames[0]->image;
    }

    return result;
}
