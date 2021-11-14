#include "SolidWall.h"
#include "Level.h"
#include "GameObjectType.h"
#include "Utils.h"


SolidWall::SolidWall() {
	_type = GameObjectType::SOLID_WALL;
	_variation = getRandomInt(1, 3);

	setHealth(3);
}

void SolidWall::render(sf::RenderWindow* rw) {
	setTextureRect(_spriteTerrain->getTextureRect());
	
	GameObject::render(rw);
}

void SolidWall::setTextureRect(sf::IntRect rect) {
	switch (getHealth()) {
	    case 2 : {
		    rect.top = 14 * PIXELS_PER_CELL;
		    break;
	    }
	    case 1 : {
		    rect.top = 15 * PIXELS_PER_CELL;
		    break;
	    }
	}
	
	_spriteTerrain->setTextureRect(rect);
}