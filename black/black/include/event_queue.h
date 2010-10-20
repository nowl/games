#ifndef __EVENT_QUEUE_H__
#define __EVENT_QUEUE_H__

void event_queue_init();
void event_queue_destroy();

void
event_queue_push(SDL_Event *event);

SDL_Event *event_queue_peek();
void event_queue_pop();

#endif // __EVENT_QUEUE_H__
