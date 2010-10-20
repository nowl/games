#ifndef __PLAYERUNIT_H__
#define __PLAYERUNIT_H__

#include <iostream>

#include "ImageBackedGameObject.h"

class PlayerUnit : public GameObject {
public:
	PlayerUnit(const std::string& name,
			   int layer,
			   const Position& position,
			   int lives,
               int scrollBorderWidth)
        : GameObject(name, layer),
          _numLives(lives),
          _scrollBorderWidth(scrollBorderWidth)
        {
            _regularImage = new ImageBackedGameObject(name, layer, position, "units/Player1.png");
            _ghostImage = new ImageBackedGameObject(name, layer, position, "units/PlayerGhost.png");

            _regularImage->setVisible(true);
            _ghostImage->setVisible(false);

            addChild(_regularImage);
            addChild(_ghostImage);
        }
    
	virtual ~PlayerUnit() {
        delete _regularImage;
        delete _ghostImage;
    }

	virtual bool processEvent(const IEvent& event,
							  GameState* gameState);

    virtual void specificUpdate(GameState* gameState);

    virtual Extents getBBox();

    virtual void setWorldPosition(const Position& position) {
        GameObject::setWorldPosition(position);
        BOOST_FOREACH(GameObject* obj, getChildren())
            obj->setWorldPosition(position);
    }

	int getLives() {
		return _numLives;
	}

	void setLives(int lives) {
		_numLives = lives;
	}

private:
    struct PlayerState {
        PlayerState()
            : movingUp(false), movingDown(false), movingLeft(false), movingRight(false),
              usingDeforium(false)
            {}
        
        bool movingUp;
        bool movingDown;
        bool movingLeft;
        bool movingRight;
        bool usingDeforium;
    };

    ImageBackedGameObject* _regularImage;
    ImageBackedGameObject* _ghostImage;

    PlayerState _playerState;
    Position _oldWorldPos;
	int _numLives;
    int _scrollBorderWidth;
};

#endif  // __PLAYERUNIT_H__
