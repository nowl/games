#include <vector>
#include <iostream>
#include <boost/any.hpp>
#include <SDL/SDL.h>

#include "PlayerUnit.h"
#include "map.h"
#include "MapGameObject.h"
#include "TextObject.h"
#include "MapBuilder.h"

using namespace std;
using namespace boost;

bool PlayerUnit::processEvent(const IEvent& event,
							  GameState* gameState)
{
    if(event.getType() == Event::KEYPRESS) {
        vector<any> data = event.getData();
        SDLKey sym = any_cast<SDLKey>(data[0]);
        
        if(sym == SDLK_LEFT) {
            _playerState.movingLeft = true;
            return true;
        }
        if(sym == SDLK_RIGHT) {
            _playerState.movingRight = true;
            return true;
        }
        if(sym == SDLK_UP) {
            _playerState.movingUp = true;
            return true;
        }
        if(sym == SDLK_DOWN) {
            _playerState.movingDown = true;
            return true;
        }
        
        if(sym == SDLK_SPACE) {
            _playerState.usingDeforium = !_playerState.usingDeforium;
                                    
            float deforium = any_cast<float>((*gameState)["Deforium"]);
            if(_playerState.usingDeforium && deforium <= 0)
                _playerState.usingDeforium = false;

            if(!_playerState.usingDeforium) {
                _regularImage->setVisible(true);            
                _ghostImage->setVisible(false);
            }

            return true;
        }
    }

    if(event.getType() == Event::KEYRELEASE) {
        vector<any> data = event.getData();
        SDLKey sym = any_cast<SDLKey>(data[0]);
        
        if(sym == SDLK_LEFT) {
            _playerState.movingLeft = false;
            return true;
        }
        if(sym == SDLK_RIGHT) {
            _playerState.movingRight = false;
            return true;
        }
        if(sym == SDLK_UP) {
            _playerState.movingUp = false;
            return true;
        }
        if(sym == SDLK_DOWN) {
            _playerState.movingDown = false;
            return true;
        }
    }
    
    if(event.getType() == Event::WALL_COLLISION && !_playerState.usingDeforium) {
        setWorldPosition(_oldWorldPos);
    }

	if(event.getType() == Event::ENEMY_COLLISION) {
		setLives(getLives() - 1);
        setWorldPosition(Position(0,0));

        MapGameObject* mgo = any_cast<MapGameObject*>((*gameState)["MapGameObject"]);
        mgo->setScrollNTicks(MapGameObject::LEFT, 1000);
        mgo->setScrollNTicks(MapGameObject::UP, 1000);
	}
        
    return false;
}

void
PlayerUnit::specificUpdate(GameState* gameState)
{
    _oldWorldPos = getWorldPosition();
    Position newWorldPos = _oldWorldPos;

    if(_playerState.movingDown)
        newWorldPos += Position(0, 2);
    if(_playerState.movingUp)
        newWorldPos += Position(0, -2);
    if(_playerState.movingRight)
        newWorldPos += Position(2, 0);
    if(_playerState.movingLeft)
        newWorldPos += Position(-2, 0);

    setWorldPosition(newWorldPos);

    // check for level completion
    if(newWorldPos.getX() > 49*32 && newWorldPos.getY() > 49*32) {
        vector<GameObject*> gameUnits;
        any_cast<MapBuilder*>((*gameState)["MapBuilder"])->populate(5.0, gameUnits);
        MapGameObject* mgo = any_cast<MapGameObject*>((*gameState)["MapGameObject"]);
        BOOST_FOREACH(GameObject* goptr, gameUnits)
            mgo->addObject(goptr);
        setWorldPosition(Position(0,0));
        mgo->setScrollNTicks(MapGameObject::LEFT, 1000);
        mgo->setScrollNTicks(MapGameObject::UP, 1000);
        (*gameState)["Level"] = any_cast<int>((*gameState)["Level"]) + 1;
    }

	MapGameObject* mgo = any_cast<MapGameObject*>((*gameState)["MapGameObject"]);
	mgo->runCollisions(gameState, this);

    // update deforium
    float deforium = any_cast<float>((*gameState)["Deforium"]);
    TextObject *tobj = any_cast<TextObject*>((*gameState)["DeforiumText"]);
    
    if(deforium < 500) {
        deforium += 0.25;
        tobj->setColor(0,0,250);
    } else {
        deforium = 500;
        tobj->setColor(250,0,0);        
    }

    if(_playerState.usingDeforium) {
        deforium -= 1;
        _regularImage->setVisible(false);
        _ghostImage->setVisible(true);           

        if(deforium <= 0) {
            _playerState.usingDeforium = false;
            _regularImage->setVisible(true);            
            _ghostImage->setVisible(false);
            deforium = 0;
        }
    }

    stringstream mess;
    mess << "Deforium: " << (int)deforium;
    tobj->setText(mess.str());
    (*gameState)["Deforium"] = deforium;

    // update score
    (*gameState)["Score"] = any_cast<float>((*gameState)["Score"]) + any_cast<int>((*gameState)["Level"])*0.01f;
    if(any_cast<float>((*gameState)["Score"]) > any_cast<int>((*gameState)["Level"]) * 100)
        (*gameState)["Score"] = any_cast<int>((*gameState)["Level"]) * 100.0f;
    stringstream mess3;
    mess3 << "Score: " << (int)any_cast<float>((*gameState)["Score"]);
    any_cast<TextObject*>((*gameState)["ScoreText"])->setText(mess3.str());

    // update lives
    tobj = any_cast<TextObject*>((*gameState)["LivesText"]);
    stringstream mess2;
    mess2 << "Lives: " << getLives();
    tobj->setText(mess2.str());

    // check if map should scroll
    int border = _scrollBorderWidth;
    
    Extents mapExtents = mgo->getExtents();
    
    int height = mapExtents.getHeight();
    int width = mapExtents.getWidth();
    int x = 0;
    int y = 0;
        
    Extents leftBB = Extents(x, y, border, height);
    Extents rightBB = Extents(x+width-border, y, border, height);
    Extents topBB = Extents(x, y, width, border);
    Extents bottomBB = Extents(x, y+height-border, width, border);
            
    int mX = getScreenPosition().getX();
    int mY = getScreenPosition().getY();
    
    const int scrollTicks = 75;

    if(pointInBB(mX, mY, leftBB))
        mgo->setScrollNTicks(MapGameObject::LEFT, scrollTicks);
    if(pointInBB(mX, mY, rightBB))
        mgo->setScrollNTicks(MapGameObject::RIGHT, scrollTicks);
    if(pointInBB(mX, mY, topBB))
        mgo->setScrollNTicks(MapGameObject::UP, scrollTicks);
    if(pointInBB(mX, mY, bottomBB))
        mgo->setScrollNTicks(MapGameObject::DOWN, scrollTicks);
}

Extents
PlayerUnit::getBBox()
{
    return Extents(getWorldPosition().getX()+10,
                   getWorldPosition().getY()+10,
                   _unitSize-20,
                   _unitSize-20);
}
