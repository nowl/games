#ifndef __EVENTPROCESSOR_H__
#define __EVENTPROCESSOR_H__

#include "GameState.h"
#include "IEvent.h"

bool processEventGlobal(GameState& gameState, const IEvent& event);

#endif	// __EVENTPROCESSOR_H__
