#include <random>

#include "TankEnemy.h"
#include "Utils.h"
#include "Wall.h"
#include "Bullet.h"
#include "GameObjectType.h"
#include "Game.h"


extern std::mt19937* RANDOM;

TankEnemy::TankEnemy(const class Game& game, sf::IntRect rect) : Tank(game) {
	setType(GameObjectType::TANK_ENEMY);

	_analizeTime = ENEMY_AI_ANALIZER_TIME;
	_analizeTimer = getRandomFloat(0.0f, _analizeTime);
	_lastAnalizeX = 0.0f;
	_lastAnalizeY = 0.0f;

	setHealth(ENEMY_HEALTH);
	setMaxSpeed(ENEMY_SPEED);
	setSpeedup(10.0f);
	setBrakingSpeed(8.0f);
	setRotationTime(0.9f);

    _spriteEntity->setTextureRect(rect);
}

TankEnemy::~TankEnemy() {
	getGame().increaseDiedEnemiesCount();
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
	calculateFrontCellPosition(x, y);

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
    std::unique_ptr<GameObject>* object{nullptr};
	float distance = 0;
	do {
		object = &getGame().checkIntersects(x, y, 0.01f, 0.01f, 0);

		x += xDelta;
		y += yDelta;
		distance += 0.01f;
	} while (!*object);

	// Защита от снаряда
	if ((*object)->getType() == GameObjectType::BULLET) {
		if (dynamic_cast<Bullet&>(**object).getOwnerType() == GameObjectType::TANK_FIRST_PLAYER
			|| dynamic_cast<Bullet&>(**object).getOwnerType() == GameObjectType::TANK_SECOND_PLAYER)
			fire();
		return;
	}

	// Атака
	if ((*object)->getType() == GameObjectType::TANK_FIRST_PLAYER
		|| (*object)->getType() == GameObjectType::TANK_SECOND_PLAYER
		|| (*object)->getType() == GameObjectType::BASE) {
		fire();
		return;
	}

	// 20% вероятность изменения направления
	if (0 == (*RANDOM)() % 5) {
		// Двигаться в случайном направлении
		moveRandomDirection(dt);
		return;
	}

	
	switch ((*object)->getType()) {
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
    Direction newDirection{Direction::NONE};
	do {
		newDirection = (Direction)((*RANDOM)() % ((int)Direction::MAX));
	} while (prevDirection == newDirection);
	
	move(newDirection, dt);
}