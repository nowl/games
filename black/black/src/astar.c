#include "black.h"

#define INITIAL_OPEN_CLOSE_SIZE 16
#define HEURISTIC_DISTANCE_MOD 10

struct astar_info {
    struct astar_open_info {
        int x;
        int y;
    } *open;
    int open_first, open_n, open_c;

    struct astar_closed_info {
        int x;
        int y;
        int cost;
        int prev_best;
    } *closed;
    int closed_n, closed_c;
};

enum {
    OPEN,
    CLOSED
};

static int
point_in_list(int x, int y, int type, struct astar_info *inf)
{
    int n;

    switch(type) {
    case OPEN:
        for(n=0; n<inf->open_n; n++)
        if(inf->open[n].x == x && inf->open[n].y == y)
            return TRUE;
        break;
    case CLOSED:
        for(n=0; n<inf->closed_n; n++)
        if(inf->closed[n].x == x && inf->closed[n].y == y)
            return TRUE;
        break;
    }

    return FALSE;
}

static void
add_point_to_list(int x, int y, int cost, int prev_best, int type, struct astar_info *inf)
{
    int n;

    switch(type) {
    case OPEN:
        n = ++inf->open_n;
        inf->open = memory_grow_to_size(inf->open, sizeof(*inf->open), &inf->open_c, inf->open_n);
        inf->open[n].x = x;
        inf->open[n].y = y;
        break;
    case CLOSED:
        n = ++inf->closed_n;
        inf->closed = memory_grow_to_size(inf->closed, sizeof(*inf->closed), &inf->closed_c, inf->closed_n);
        inf->closed[n].x = x;
        inf->closed[n].y = y;
        inf->closed[n].cost = cost;
        inf->closed[n].prev_best = prev_best;
        break;
    }
}

static struct astar_open_info*
pop_open_list(struct astar_info *inf)
{
    if(inf->open_first == inf->open_n)
        return NULL;

    return &inf->open[inf->open_first++];
}

static float
heuristic_dist(float *costs, int x, int y, int dest_x, int dest_y)
{
    // just use manhattan distance for now

    return HEURISTIC_DISTANCE_MOD * (abs(dest_x - x) + abs(dest_y - y));
}

static int *
backtrack_to_get_path(struct astar_info *inf, int *length)
{
    // count links to get exact alloc amount

    int n = 1;
    struct astar_closed_info *c = &inf->closed[inf->closed_n-1];
    while(c->prev_best != -1)
    {
        c = &inf->closed[c->prev_best];
        ++n;
    }

    // backtrack

    int *path = malloc(sizeof(*path) * 2 * n);
    *length = 2*n;

    n = 0;
    c = &inf->closed[inf->closed_n-1];
    while(c->prev_best != -1)
    {
        path[n++] = c->x;
        path[n++] = c->y;
        c = &inf->closed[c->prev_best];
    }

    path[n++] = c->x;
    path[n++] = c->y;

    return path;
}

void
astar_find_path(float *costs, int width, int height,
                int x, int y, int dest_x, int dest_y,
                int *path, int *path_length)
{
    // create ainfo structure

    struct astar_info ainfo = {NULL, 0, 0, 0, NULL, 0, 0};
    ainfo.open = memory_grow_to_size(ainfo.open, sizeof(*ainfo.open), &ainfo.open_c, INITIAL_OPEN_CLOSE_SIZE);
    ainfo.closed = memory_grow_to_size(ainfo.closed, sizeof(*ainfo.closed), &ainfo.closed_c, INITIAL_OPEN_CLOSE_SIZE);

    // add start position to open list

    add_point_to_list(x, y, -1, -1, OPEN, &ainfo);

    // free memory

    free(ainfo.open);
    free(ainfo.closed);
}
