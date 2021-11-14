#include "TankPlayer.h"
#include "Utils.h"
#include "GameObjectType.h"
#include "Level.h"



TankPlayer::TankPlayer() {
	_type = GameObjectType::TANK_FIRST_PLAYER;

	_keyLeft = 0;
	_keyRight = 0;
	_keyUp = 0;
	_keyDown = 0;
	_keyFire = 0;

	setHealth(PLAYER_HEALTH);
	setMaxSpeed(PLAYER_SPEED);
	setSpeedup(12.0f);
	setBrakingSpeed(6.5f);
	setRotationTime(0.76f);

	setDestroyAfterDeath(false);
}

void TankPlayer::setTextureRect(sf::IntRect rect) {
	_spriteEntity->setTextureRect(rect);
}

void TankPlayer::update(float dt) {
	if (isKeyDown(_keyFire))
		fire();
	
	if (isKeyDown(_keyLeft))
		move(Direction::LEFT, dt);

	else if (isKeyDown(_keyRight))
		move(Direction::RIGHT, dt);

	else if (isKeyDown(_keyUp))
		move(Direction::UP, dt);

	else if (isKeyDown(_keyDown))
		move(Direction::DOWN, dt);

	else
		move(Direction::NONE, dt);

	Tank::update(dt);
}

void  TankPlayer::setKeys(int keyLeft, int keyRight, int keyUp, int keyDown, int keyFire) {
	_keyLeft = keyLeft;
	_keyRight = keyRight;
	_keyUp = keyUp;
	_keyDown = keyDown;
	_keyFire = keyFire;
}