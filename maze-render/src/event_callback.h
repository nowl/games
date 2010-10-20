#ifndef EVENT_CALLBACK_H__
#define EVENT_CALLBACK_H__

void event_callback_add(SDLEventCallback callback_func, unsigned int mode);
void event_callback_remove(SDLEventCallback callback_func, unsigned int mode);

#endif	/* EVENT_CALLBACK_H__ */
