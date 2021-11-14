#include "TankEnemy.h"
#include "Utils.h"
#include "Wall.h"
#include "Bullet.h"
#include "GameObjectType.h"
#include "Game.h"
#include "Level.h"


TankEnemy::TankEnemy() {
	_type = GameObjectType::TANK_ENEMY;
	_analizeTime = ENEMY_AI_ANALIZER_TIME;
	_analizeTimer = getRandomFloat(0.0, _analizeTime);
	_lastAnalizeX = 0.0;
	_lastAnalizeY = 0.0;

	setHealth(ENEMY_HEALTH);
	setMaxSpeed(ENEMY_SPEED);
	setSpeedup(10.0f);
	setBrakingSpeed(8.0f);
	setRotationTime(0.9f);
}

TankEnemy::~TankEnemy() {
	_game->increaseDiedEnemiesCount();
}

void TankEnemy::setTextureRect(sf::IntRect rect) {
	_spriteEntity->setTextureRect(rect);
}

void TankEnemy::update(float dt) {
	Tank::update(dt);

	_analizeTimer += dt;
	if (_analizeTimer > _analizeTime) {
		_analizeTimer = 0;
		analize(dt);
	}
}

void TankEnemy::analize(float dt) {
	float x = 0.0;
	float y = 0.0;
	calculateFrontCellPosition(&x, &y);

	float xDelta = 0.0;
	float yDelta = 0.0;

	switch (getDirection()) {
	    case Direction::LEFT : {
		    xDelta = -1.0f;
		    break;
	    }
		case Direction::RIGHT : {
			xDelta = 1.0f;
			break;
		}
		case Direction::UP : {
			yDelta = -1.0f;
			break;
		}
		case Direction::DOWN : {
			yDelta = 1.0f;
			break;
		}
	}

	// Поиск ближайших объектов с дистанцией до них
	GameObject* object = 0;
	float distance = 0;
	do {
		object = _game->checkIntersects(x, y, 0.01f, 0.01f, 0);

		x += xDelta;
		y += yDelta;
		distance += 0.01f;
	} while (object == 0);

	// Защита от снаряда
	if (object->getType() == GameObjectType::BULLET) {
		if (((class Bullet*)object)->getOwnerType() == GameObjectType::TANK_FIRST_PLAYER
			|| ((class Bullet*)object)->getOwnerType() == GameObjectType::TANK_SECOND_PLAYER)
			fire();
		return;
	}

	// Атака
	if (object->getType() == GameObjectType::TANK_FIRST_PLAYER
		|| object->getType() == GameObjectType::TANK_SECOND_PLAYER
		|| object->getType() == GameObjectType::BASE) {
		fire();
		return;
	}

	// 20% вероятность изменения направления
	if (0 == rand() % 5) {
		// Двигаться в случайном направлении
		moveRandomDirection(dt);
		return;
	}

	
	switch (object->getType()) {
	    case GameObjectType::WALL : {
		    if (((class Wall*)object)->getInvulnerable() == false) {
			    fire();
			    return;
		    }
		    break;
	    }
	}

	
	// Смена направления движения
	bool conditionOne = distance == 1;
	bool conditionTwo = _lastAnalizeX == getX();
	bool conditionTree = _lastAnalizeY == getY();
	
	if (conditionOne || (conditionTwo && conditionTree)) {
		// Двигаться в случайном направлении
		moveRandomDirection(dt);
		return;
	}

	
	_lastAnalizeX = getX();
	_lastAnalizeY = getY();
}

void TankEnemy::moveRandomDirection(float dt) {
	Direction prevDirection = getDirection();
	Direction newDirection;
	do {
		newDirection = (Direction)(rand() % ((int)Direction::MAX));
	} while (prevDirection == newDirection);
	
	move(newDirection, dt);
}