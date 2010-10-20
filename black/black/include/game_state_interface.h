#ifndef __GAMESTATEINTERFACE_H__
#define __GAMESTATEINTERFACE_H__

#include "game_manager.h"

struct game_state_interface {
public:
    virtual void Init() = 0;
    virtual void Cleanup() = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void handleEvents(GameManager *manager) = 0;
    virtual void update(GameManager *manager, unsigned int ticks) = 0;
    virtual void render(GameManager *manager, float interpolation) = 0;

protected:
    GameStateInterface() {}
    virtual ~GameStateInterface() {}
    GameStateInterface(const GameStateInterface& interface);
    GameStateInterface& operator=(const GameStateInterface& interface);
};

#endif /* __GAMESTATEINTERFACE_H__ */
