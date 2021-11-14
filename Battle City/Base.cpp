#include "Base.h"
#include "GameObjectType.h"
#include "Level.h"


Base::Base() {
	_type = GameObjectType::BASE;

	setWidth(BASE_COLUMNS);
	setHeight(BASE_ROWS);

	setHealth(BASE_HEALTH);
	setDestroyAfterDeath(false);

	_spriteTerrain = new sf::Sprite();
	_spriteTerrain->setTexture(*_atlasTerrain);
}

Base::~Base() {
	if (_spriteTerrain) {
		delete _spriteTerrain;
		_spriteTerrain = nullptr;
	}
}

void Base::render(sf::RenderWindow* rw) {
	GameObject::render(rw);
}

void Base::setTextureRect(sf::IntRect rect) {
	_spriteTerrain->setTextureRect(rect);
}