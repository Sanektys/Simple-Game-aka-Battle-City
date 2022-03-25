#include "TankPlayer.h"


TankPlayer::TankPlayer() {
	_type = GameObjectType::TANK_FIRST_PLAYER;

	setHealth(PLAYER_HEALTH);
	setMaxSpeed(PLAYER_SPEED);
	setSpeedup(12.0f);
	setBrakingSpeed(6.5f);
	setRotationTime(0.76f);

	setDestroyAfterDeath(false);
}

void TankPlayer::update(float dt) {
	if (sf::Keyboard::isKeyPressed(_keyFire))
		fire();
	
	if (sf::Keyboard::isKeyPressed(_keyLeft))
		move(Direction::LEFT, dt);

	else if (sf::Keyboard::isKeyPressed(_keyRight))
		move(Direction::RIGHT, dt);

	else if (sf::Keyboard::isKeyPressed(_keyUp))
		move(Direction::UP, dt);

	else if (sf::Keyboard::isKeyPressed(_keyDown))
		move(Direction::DOWN, dt);

	else
		move(Direction::NONE, dt);

	Tank::update(dt);
}

void TankPlayer::setTextureRect(sf::IntRect rect) {
	_spriteEntity->setTextureRect(rect);
}

void  TankPlayer::setKeys(sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight,
						  sf::Keyboard::Key keyUp, sf::Keyboard::Key keyDown,
						  sf::Keyboard::Key keyFire) {
	_keyLeft  = keyLeft;
	_keyRight = keyRight;
	_keyUp    = keyUp;
	_keyDown  = keyDown;
	_keyFire  = keyFire;
}