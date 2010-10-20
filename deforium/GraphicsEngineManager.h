#ifndef __GRAPHICS_ENGINE_MANAGER_H__
#define __GRAPHICS_ENGINE_MANAGER_H__

#include <boost/noncopyable.hpp>

#include "SDLManager.h"
#include "GraphicsEngine.h"
#include "SDLGraphicsEngine.h"
#include "NoopGraphicsEngine.h"

class GraphicsEngineManager : private boost::noncopyable {
public:
    enum {SDL, NOOP};

    GraphicsEngineManager(int type,
                          SubSystemManager * manager)
        {
            switch(type) {
            case SDL:
                mGE = new SDLGraphicsEngine((SDLManager*)manager);
                break;
			case NOOP:
				mGE = new NoopGraphicsEngine();
                break;
            }
        }

    ~GraphicsEngineManager() {
        delete mGE;
    }

    GraphicsEngine *getEngine() {
        return mGE;
    }

private:
    GraphicsEngine *mGE;
};

#endif  // __GRAPHICS_ENGINE_MANAGER_H__
