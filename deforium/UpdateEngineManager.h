#ifndef __UPDATE_ENGINE_MANAGER_H__
#define __UPDATE_ENGINE_MANAGER_H__

#include <boost/noncopyable.hpp>

#include "SubSystemManager.h"
#include "UpdateEngine.h"
#include "GraphicsEngine.h"
#include "SDLUpdateEngine.h"
#include "GameState.h"

class UpdateEngineManager : private boost::noncopyable {
public:
    enum {SDL};

    UpdateEngineManager(int type, 
                        SubSystemManager * manager,
                        const GraphicsEngine& ge,
                        GameState& gs)
    {
        switch(type) {
        case SDL:
            mUE = new SDLUpdateEngine((SDLManager*)manager, ge, gs);
            break;
        }
    }

    ~UpdateEngineManager() {
        delete mUE;
    }

    UpdateEngine *getEngine() {
        return mUE;
    }

private:
    UpdateEngine *mUE;
};

#endif  // __UPDATE_ENGINE_MANAGER_H__
