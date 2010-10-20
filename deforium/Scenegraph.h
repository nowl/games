#ifndef __SCENEGRAPH_H__
#define __SCENEGRAPH_H__

#include "GameObject.h"

class Scenegraph : public GameObject
{
public:
	Scenegraph();

    virtual bool processEvent(const IEvent& event,
							  GameState* gameState);

    // debug
    void printChildrenRecursive();
};

#endif  // __SCENEGRAPH_H__
