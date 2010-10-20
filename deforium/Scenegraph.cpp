#include <SDL/SDL.h>

#include <iostream>
#include <vector>
#include <boost/any.hpp>

#include "Scenegraph.h"
#include "TextObject.h"

using namespace std;
using namespace boost;

Scenegraph::Scenegraph() : GameObject("Scenegraph", -1) {}

bool Scenegraph::processEvent(const IEvent& event,
							  GameState* gameState)
{
    if(event.getType() == Event::KEYPRESS) {
        vector<any> data = event.getData();
        SDLKey sym = any_cast<SDLKey>(data[0]);
        
        if(sym == SDLK_q) {
            gameState->mainLoopRunning = false;
            return true;
        } else if(sym == SDLK_BACKQUOTE) {
			TextObject *obj = any_cast<TextObject*>((*gameState)["FPSText"]);
            obj->setVisible(!obj->getVisible());
            return true;
        }
    }

    return false;
}

static int counter = 0;
static int total = 0;

void
printChildrenRecursiveAux(GameObject* obj)
{
    for(int i=0; i<counter; i++)
        cout << " ";
    cout << "obj->getName()" << endl;
    total++;

    BOOST_FOREACH(GameObject* child, obj->getChildren()) {
        counter++;
        printChildrenRecursiveAux(child);
        counter--;
    }
}

void
Scenegraph::printChildrenRecursive()
{
    printChildrenRecursiveAux(this);

    cout << "total objects = " << total << endl;
}
