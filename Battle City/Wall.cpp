#include "Wall.h"
#include "Level.h"
#include "Utils.h"
#include "GameObjectType.h"


Wall::Wall() {
	_type = GameObjectType::WALL;
	_variation = getRandomInt(1, 3);

	_spriteTerrain = new sf::Sprite();
	_spriteTerrain->setTexture(*_atlasTerrain);
	_spriteTerrain->setOrigin((getWidth() * (float)PIXELS_PER_CELL) / 2.0f,
		                      (getHeight() * (float)PIXELS_PER_CELL) / 2.0f);
}

Wall::~Wall() {
	if (_spriteTerrain) {
		delete _spriteTerrain;
		_spriteTerrain = nullptr;
	}
}

void Wall::render(sf::RenderWindow* rw) {
	GameObject::render(rw);
}

void Wall::setTextureRect(sf::IntRect rect) {
	switch (_variation) {
	    case 2 : {
		    rect.left = PIXELS_PER_CELL;
		    break;
	    }
		case 3 : {
			rect.left = 2 * PIXELS_PER_CELL;
			break;
		}
	}

	_spriteTerrain->setTextureRect(rect);
}