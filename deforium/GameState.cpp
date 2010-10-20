#include "GameState.h"

GameState::GameState() 
    : mainLoopRunning(true), normalUnitSize(32)
{}

/*
using namespace std;

ostream& operator<<(ostream& out, 
					const GameState& gameState)
{
	return out << gameState.toString();
}

string GameState::toString() const
{
	map<string, boost::any>::const_iterator iter;
	stringstream result;
	
	result << "{";
	for(iter=_res.begin(); iter!= _res.end(); iter++) {
		result << "\"" << iter->first << "\": " << &iter->second << ", ";
	}
	result << "}";

	return result.str();
}
*/
