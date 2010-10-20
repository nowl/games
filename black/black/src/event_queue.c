#include "black.h"

#define MAX_EVENT_QUEUE_SIZE 64

static SDL_Event *event_queue[MAX_EVENT_QUEUE_SIZE];
static int next = 0;
static int first = -1;

void
event_queue_init()
{
    int i=0;

    for(i=0; i<MAX_EVENT_QUEUE_SIZE; i++)
        event_queue[i] = malloc(sizeof(*event_queue[i]));
}

void
event_queue_destroy()
{
    int i=0;

    for(i=0; i<MAX_EVENT_QUEUE_SIZE; i++)
        free(event_queue[i]);
}

void
event_queue_push(SDL_Event *event)
{
    if(first == -1)
        first = 0;

    *event_queue[next] = *event;
    next++;
    next %= MAX_EVENT_QUEUE_SIZE;
    if(next == first)
        LOG("exceeded max event_queue size\n");
}

SDL_Event *
event_queue_peek()
{
    if(first < 0)
        return NULL;

    return event_queue[first];
}

void
event_queue_pop()
{
    if(first < 0)
        return;

    first++;
    first %= MAX_EVENT_QUEUE_SIZE;
    if(first == next) {
        first = -1;
        next = 0;
    }
}
