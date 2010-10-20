#ifndef __SDL_SOUND__
#define __SDL_SOUND__

#include "SDL_mixer.h"

int sdl_sound_load(char *file, char *alias);
void sdl_play_sound(char *file);
void sound_loader_cleanup();

#endif  /* __SDL_SOUND__ */
