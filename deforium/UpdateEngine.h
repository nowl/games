#ifndef __UPDATE_ENGINE_H__
#define __UPDATE_ENGINE_H__

#include "GraphicsEngine.h"

class UpdateEngine {
public:
    UpdateEngine(const GraphicsEngine& ge) : mGraphicsEngine(ge) {};
    
    virtual ~UpdateEngine() {};

    virtual void update() = 0;

private:
    // non-copyable
    UpdateEngine(const UpdateEngine&);
    //UpdateEngine operator=(const UpdateEngine&);
    
    const GraphicsEngine& mGraphicsEngine;
};

#endif	// __UPDATE_ENGINE_H__
