#include "SDLUpdateEngine.h"

#include "GameState.h"
#include "Event.h"
#include "EventProcessor.h"

#include <SDL/SDL.h>

void
SDLUpdateEngine::update()
{
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_KEYDOWN: {
            //Uint8 type = event.key.type;
            //Uint8 state = event.key.state;
            SDLKey sym = event.key.keysym.sym;

            std::vector< boost::any > data;
            data.push_back(sym);
            Event newEvent(Event::KEYPRESS, data);
            processEventGlobal(mGameState, newEvent);

            break;
        }

        case SDL_KEYUP: {
            SDLKey sym = event.key.keysym.sym;

            std::vector< boost::any > data;
            data.push_back(sym);
            Event newEvent(Event::KEYRELEASE, data);
            processEventGlobal(mGameState, newEvent);

            break;
        }

        case SDL_MOUSEBUTTONDOWN: {
            Uint8 sdlButton = event.button.button;
            Event::MouseButton button;
            switch(sdlButton) {
            case SDL_BUTTON_LEFT:
                button = Event::LEFT;
                break;
            case SDL_BUTTON_MIDDLE:
                button = Event::MIDDLE;
                break;
            case SDL_BUTTON_RIGHT:
                button = Event::RIGHT;
                break;
            case SDL_BUTTON_WHEELUP:
                button = Event::WHEELUP;
                break;
            case SDL_BUTTON_WHEELDOWN:
                button = Event::WHEELDOWN;
                break;
            }
            
            unsigned int x = event.button.x;
            unsigned int y = event.button.y;

            std::vector< boost::any > data;
            data.push_back(button);
            data.push_back(x);
            data.push_back(y);
            Event newEvent(Event::MOUSE_PRESS, data);
            processEventGlobal(mGameState, newEvent);

            break;            
        }

        case SDL_MOUSEBUTTONUP: {
            Uint8 sdlButton = event.button.button;
            Event::MouseButton button;
            switch(sdlButton) {
            case SDL_BUTTON_LEFT:
                button = Event::LEFT;
                break;
            case SDL_BUTTON_MIDDLE:
                button = Event::MIDDLE;
                break;
            case SDL_BUTTON_RIGHT:
                button = Event::RIGHT;
                break;
            case SDL_BUTTON_WHEELUP:
                button = Event::WHEELUP;
                break;
            case SDL_BUTTON_WHEELDOWN:
                button = Event::WHEELDOWN;
                break;
            }
            
            unsigned int x = event.button.x;
            unsigned int y = event.button.y;

            std::vector< boost::any > data;
            data.push_back(button);
            data.push_back(x);
            data.push_back(y);
            Event newEvent(Event::MOUSE_RELEASE, data);
            processEventGlobal(mGameState, newEvent);

            break;
        }
        case SDL_MOUSEMOTION: {
            unsigned int x = event.motion.x;
            unsigned int y = event.motion.y;
            unsigned int xrel = event.motion.xrel;
            unsigned int yrel = event.motion.yrel;

            std::vector< boost::any > data;
            data.push_back(x);
            data.push_back(y);
            data.push_back(xrel);
            data.push_back(yrel);
            Event newEvent(Event::MOUSE_MOVE, data);
            processEventGlobal(mGameState, newEvent);

            break;
        }
        default:
            break;
        }
    }
}
