#ifndef __SDL_UPDATE_ENGINE_H__
#define __SDL_UPDATE_ENGINE_H__

#include "GraphicsEngine.h"
#include "UpdateEngine.h"
#include "SDLManager.h"
#include "GameState.h"

class SDLUpdateEngine : public UpdateEngine {
public:
    SDLUpdateEngine(SDLManager * sdl,
                    const GraphicsEngine& ge,
                    GameState& gs)
        : UpdateEngine(ge), mSDLMgr(sdl), mGameState(gs)
        {}

    virtual ~SDLUpdateEngine() {}

    virtual void update();
  
private:
    SDLManager *mSDLMgr;
    GameState &mGameState;
};

#endif	// __SDL_UPDATE_ENGINE_H__
