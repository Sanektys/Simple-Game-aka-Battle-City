#include "GameObject.h"
#include "Level.h"
#include "Game.h"
#include "CoordPoint.h"
#include "Utils.h"


GameObject::GameObject() {
	_game = 0;
	_type = GameObjectType::NONE;

	_x = 0.0;
	_y = 0.0;
	_xSpeed = 0.0;
	_ySpeed = 0.0;

	_width = 1;
	_height = 1;

	_health = 1;
	_destroyAfterDeath = true;
	_invulnerable = false;

	_physical = true;

	_direction = Direction::UP;

	_spriteTerrain = nullptr;
	_spriteEntity  = nullptr;

	_isSticking = false;
	_inBypass = false;
}

void GameObject::setTextureRect(sf::IntRect rect) {
	
}

void GameObject::render(sf::RenderWindow* rw) {
	if (_spriteTerrain || _spriteEntity) {
		float row = _y;
		float column = _x;

		if (_spriteTerrain) {
			_spriteTerrain->setPosition(column * PIXELS_PER_CELL + (getWidth() * PIXELS_PER_CELL) / 2.0f,
				                        row * PIXELS_PER_CELL + (getHeight() * PIXELS_PER_CELL) / 2.0f);
			//_spriteTerrain->setScale(2, 2);
			rw->draw(*_spriteTerrain);
		}
		else if (_spriteEntity) {
			_spriteEntity->setPosition(column * PIXELS_PER_CELL + (getWidth() * PIXELS_PER_CELL) / 2.0f,
				                       row * PIXELS_PER_CELL + (getHeight() * PIXELS_PER_CELL) / 2.0f);
			rw->draw(*_spriteEntity);
		}
	}
}

void GameObject::update(float dt) {
	switch (_type) {
	    case GameObjectType::TANK_ENEMY :
	    case GameObjectType::TANK_FIRST_PLAYER :
		case GameObjectType::TANK_SECOND_PLAYER : {
			if (!_inBypass)
			    _isSticking = _game->checkIntersects(_x, _y, _width, _height, this);

			if (_isSticking)
				escapeSticking();
		}
		
		case GameObjectType::BULLET : {
			float oldRow = _y;
			float oldColumn = _x;

			float newY = _y + _ySpeed * dt;
			float newX = _x + _xSpeed * dt;

			float newRow = newY;
			float newColumn = newX;

			if ((oldColumn != newColumn) && !_isSticking && !_inBypass)
				_game->moveObjectTo(this, newX, _y);
			else 
				_x = newX;

			if ((oldRow != newRow) && !_isSticking && !_inBypass)
				_game->moveObjectTo(this, _x, newY);
			else
				_y = newY;

			break;
		}
	}
}

void GameObject::intersect(class GameObject* object) {

}

//
void GameObject::escapeSticking() {
		//
		CoordPoint topLeftPoint;
		CoordPoint topRightPoint;
		CoordPoint bottomRightPoint;
		CoordPoint bottomLeftPoint;
		
		topLeftPoint.setCoordPoint(this, typeCoordPoint::TOP_LEFT);
		topRightPoint.setCoordPoint(this, typeCoordPoint::TOP_RIGHT);
		bottomRightPoint.setCoordPoint(this, typeCoordPoint::BOTTOM_RIGHT);
		bottomLeftPoint.setCoordPoint(this, typeCoordPoint::BOTTOM_LEFT);
		
		//
		bool topLeftIntersect = _game->checkIntersects(topLeftPoint.coordX, topLeftPoint.coordY,
			                                           topLeftPoint.width, topLeftPoint.height,
			                                           this);
		bool topRightIntersect = _game->checkIntersects(topRightPoint.coordX, topRightPoint.coordY,
			                                            topRightPoint.width, topRightPoint.height,
			                                            this);
		bool bottomRightIntersect = _game->checkIntersects(bottomRightPoint.coordX, bottomRightPoint.coordY,
			                                               bottomRightPoint.width, bottomRightPoint.height,
			                                               this);
		bool bottomLeftIntersect = _game->checkIntersects(bottomLeftPoint.coordX, bottomLeftPoint.coordY,
			                                              bottomLeftPoint.width, bottomLeftPoint.height,
			                                              this);

		bool primarySet = false;
		float shiftSpeed = 0.6f;

		//
		if (topLeftIntersect && topRightIntersect) {
			if (getYSpeed() < shiftSpeed)
				setYSpeed(shiftSpeed);
			else
				setYSpeed(getYSpeed());
			primarySet = true;
		}
		if (topRightIntersect && bottomRightIntersect) {
			if (getXSpeed() > -shiftSpeed)
				setXSpeed(-shiftSpeed);
			else
				setXSpeed(getXSpeed());
			primarySet = true;
		}
		if (bottomRightIntersect && bottomLeftIntersect) {
			if (getYSpeed() > -shiftSpeed)
				setYSpeed(-shiftSpeed);
			else
				setYSpeed(getYSpeed());
			primarySet = true;
		}
		if (bottomLeftIntersect && topLeftIntersect) {
			if (getXSpeed() < shiftSpeed)
				setXSpeed(shiftSpeed);
			else
				setXSpeed(getXSpeed());
			primarySet = true;
		}

		if (primarySet)
			return;

		//
		if (topLeftIntersect) {
			switch (getDirection()) {
			    case Direction::UP :
				case Direction::DOWN : {
					if (getXSpeed() < shiftSpeed)
						setXSpeed(shiftSpeed);
					else
						setXSpeed(getXSpeed());
					break;
				}
				case Direction::LEFT :
				case Direction::RIGHT : {
					if (getYSpeed() < shiftSpeed)
						setYSpeed(shiftSpeed);
					else
						setYSpeed(getYSpeed());
					break;
				}
			}
		}
		else if (topRightIntersect) {
			switch (getDirection()) {
			    case Direction::UP :
				case Direction::DOWN : {
					if (getXSpeed() > -shiftSpeed)
						setXSpeed(-shiftSpeed);
					else
						setXSpeed(getXSpeed());
					break;
				}
			    case Direction::LEFT :
				case Direction::RIGHT : {
					if (getYSpeed() < shiftSpeed)
						setYSpeed(shiftSpeed);
					else
						setYSpeed(getYSpeed());
					break;
				}
			}
		}
		else if (bottomRightIntersect) {
			switch (getDirection()) {
			    case Direction::UP :
				case Direction::DOWN : {
					if (getXSpeed() > -shiftSpeed)
						setXSpeed(-shiftSpeed);
					else
						setXSpeed(getXSpeed());
					break;
				}
			    case Direction::LEFT :
				case Direction::RIGHT : {
					if (getYSpeed() > -shiftSpeed)
						setYSpeed(-shiftSpeed);
					else
						setYSpeed(getYSpeed());
					break;
				}
			}
		}
		else if (bottomLeftIntersect) {
			switch (getDirection()) {
			    case Direction::UP :
				case Direction::DOWN : {
					if (getXSpeed() < shiftSpeed)
						setXSpeed(shiftSpeed);
					else
						setXSpeed(getXSpeed());
					break;
				}
			    case Direction::LEFT :
				case Direction::RIGHT : {
					if (getYSpeed() > -shiftSpeed)
						setYSpeed(-shiftSpeed);
					else
						setYSpeed(getYSpeed());
					break;
				}
			}
		}
}

void GameObject::doDamage(int damage) {
	if (getInvulnerable())
		return;

	if (_health > damage)
		_health -= damage;
	else
		_health = 0;
}