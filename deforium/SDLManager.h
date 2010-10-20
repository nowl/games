#ifndef __SDL_MANAGER_H__
#define __SDL_MANAGER_H__

#include <SDL.h>
#include <SDL_ttf.h>

#include "DefException.h"
#include "SubSystemManager.h"

class SDLManager : public SubSystemManager {
public:
    SDLManager(Uint32 flags = SDL_INIT_VIDEO) {
        int result = SDL_Init(flags);

        if(result != 0)
            throw DefException("SDL failed to initialize");

		result = TTF_Init();

        if(result != 0)
            throw DefException(std::string("TTF failed to initialize: ") + TTF_GetError());
    }

    virtual ~SDLManager() {
		TTF_Quit();
        SDL_Quit();
    }
};

#endif // __SDL_MANAGER_H__
