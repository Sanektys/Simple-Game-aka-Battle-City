#include "Tank.h"
#include "Bullet.h"
#include "GameObjectType.h"
#include "Game.h"
#include "CoordPoint.h"


Tank::Tank(const class Game& game) : GameObject(game) {
    setGroup(GameObjectGroup::ENTITY);

	setWidth(TANK_WIDTH);
	setHeight(TANK_HEIGHT);
	_offset = (getHeight() - getWidth()) / 2.0f;

	_maxSpeed = 10.0f;
	_speedup = 12.0f;
	_currentSpeed = 0.0f;
	_brakingSpeed = 7.0f;

	_fireCooldownTime = 0.0f;

	_rotation = false;
	_rotationTime = 0.8f;
	_rotationAngle = 90.0f * (float)getDirection();
	_currentRotationTime = 0.0f;

	_oldDirection = Direction::UP;
	_inertiaDirection = Direction::LEFT;

	_spriteEntity.reset(new sf::Sprite());
	_spriteEntity->setTexture(*ATLAS_ENTITY);
	_spriteEntity->setOrigin(32, 52);

	_currentTrackShift = 0.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Обновление состояния танка каждый игровой такт

void Tank::update(float dt) {
	// При смене направления движения на 90 градусов производится обновление габаритов
	// танка вместе с его координатной точкой. Также запускается процесс вращения спрайта
	if ((_oldDirection != getDirection()) && (_rotation == false)) {
		_rotation = true;

		switch (getDirection()) {
		// Если направления отличаются на 180 градусов, то производить перерасчёт габаритов не нужно	
		case Direction::UP :
				if (_oldDirection == Direction::DOWN)
					break;
			case Direction::DOWN : {
				if (_oldDirection == Direction::UP)
					break;
				
				float newY = getY() - _offset;
				float newX = getX() + _offset;
				float newWidth = TANK_WIDTH;
				float newHeight = TANK_HEIGHT;

				bool topIntersects = bool(getGame().checkIntersects(newX, newY, newWidth, _offset, this));
				bool bottomIntersects = bool(getGame().checkIntersects(newX, getY() + getHeight(), newWidth, _offset, this));
					
				// Если танк зажат сверху и снизу, и его новые габариты не помещаются на текущем месте,
				// то отменить смену направления и запуск вращения
				if (topIntersects && bottomIntersects) {
					setDirection(_oldDirection);
					_rotation = false;
				}
				
				if (_rotation) {
					setY(newY);
					setX(newX);
					setWidth(newWidth);
					setHeight(newHeight);
				}
				break;
			}

			case Direction::RIGHT :
				if (_oldDirection == Direction::LEFT)
					break;
			case Direction::LEFT : {
				if (_oldDirection == Direction::RIGHT)
					break;
					
				float newY = getY() + _offset;
				float newX = getX() - _offset;
				float newWidth = TANK_HEIGHT;
				float newHeight = TANK_WIDTH;

				bool rightIntersects = bool(getGame().checkIntersects(getX() + getWidth(),newY, _offset, newHeight, this));
				bool leftIntersects = bool(getGame().checkIntersects(newX, newY, _offset, newHeight, this));

				if (rightIntersects && leftIntersects) {
					setDirection(_oldDirection);
					_rotation = false;
				}
				
				if (_rotation) {
					setY(newY);
					setX(newX);
					setWidth(newWidth);
					setHeight(newHeight);
				}
				break;
			}
		}		
	}

	if (_rotation) {
		if (!rotation(dt)) {
			_rotation = false;
			_currentSpeed = 0.0f;
			_oldDirection = getDirection();
		}
	}
	else {
		_oldDirection = getDirection();
	}

	if (_fireCooldownTime > 0)
		_fireCooldownTime -= dt;

	GameObject::update(dt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Отрисовка танка

void Tank::renderTracksMoving() {
	int TRACK_SHIFT_INTERVAL = 25;

	_currentTrackShift += _currentSpeed;
	
	if (_currentTrackShift >= TRACK_SHIFT_INTERVAL) {
		_currentTrackShift = 0.0f;

		sf::IntRect textureRect = _spriteEntity->getTextureRect();
		// Переход на соседний спрайт по горизонтали в атласе
		if (0 == textureRect.left) {
			textureRect.left = 64;
			_spriteEntity->setTextureRect(textureRect);
		}
		else {
			textureRect.left = 0;
			_spriteEntity->setTextureRect(textureRect);
		}
	}
}

void Tank::render(sf::RenderWindow* rw) {
	renderTracksMoving();

	GameObject::render(rw);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Расчёты по установке текущей скорости танка на каждом игровом такте

void Tank::move(enum Direction direction, float dt) {
	if ((_rotation) && ((_currentSpeed > 0) || (_currentSpeed < 0)))
		direction = Direction::NONE;
	else if (_rotation)
		return;

	// Отсутствующее направление не устанавливается как параметр для танка,
	// оно необходимо только для временного использования в пределах данной функции
	if (direction != Direction::NONE)
	    setDirection(direction);
	
	if (!_rotation)
		if (getDirection() != _oldDirection) {
			// При смене траектории движения, инерция устанавливается
			// по направлению, перпендикулярно направленному к текущему
			switch (getDirection()) { 
			    case Direction::UP :
			    case Direction::DOWN : {
					if (Direction::UP == _oldDirection
						|| Direction::DOWN == _oldDirection)
						break;
					else
						_inertiaDirection = _oldDirection;
					break;
			    }
			    case Direction::LEFT :
			    case Direction::RIGHT : {
					if (Direction::LEFT == _oldDirection
						|| Direction::RIGHT == _oldDirection)
						break;
					else
						_inertiaDirection = _oldDirection;
					break;
			    }
			}
			return;
		}

	if (getIsSticking())
		direction = Direction::NONE;

	if (direction != Direction::NONE)
		_currentSpeed += _speedup * dt;

	if (_currentSpeed > _maxSpeed)
		_currentSpeed = _maxSpeed;

	switch (direction) {
	    case Direction::UP : {
			// При inBypass = true проверка на пересечение других объектов танком будет игнорироваться
			setInBypass(bypassObstruction());
		    setYSpeed(-_currentSpeed);
		    break;
	    }
		case Direction::DOWN : {
			setInBypass(bypassObstruction());
			setYSpeed(_currentSpeed);
			break;
		}
		case Direction::LEFT : {
			setInBypass(bypassObstruction());
			setXSpeed(-_currentSpeed);
			break;
		}
		case Direction::RIGHT : {
			setInBypass(bypassObstruction());
			setXSpeed(_currentSpeed);
			break;
		}
		// При отсутствующем направлении не производится увеличение скорости.
		// Скорость со временем уменьшается, танк либо полностью останавливается,
		// либо выталкивается из пространства другого объекта, если танк застрял
		case Direction::NONE : {
			setInBypass(false);

			// При повороте танка этот блок временно выполняет роль инерции по
			// прошлому направлению
			switch (_oldDirection) {
			    case Direction::UP : {
					// Если (при повороте) на прошлой позиции танк "буксовал",
					// то его потенциальная скорость сбрасывается для
					// недопущения резкого сдвига на освободившееся пространство
					// при изменении габаритов танка
					if ((getYSpeed() == 0.0f) && (_currentSpeed >= 0.0f))
						_currentSpeed = 0.0f;
				    setYSpeed(-_currentSpeed);
				    break;
			    }
				case Direction::DOWN : {
					if ((getYSpeed() == 0.0f) && (_currentSpeed >= 0.0f))
						_currentSpeed = 0.0f;
					setYSpeed(_currentSpeed);
					break;
				}
				case Direction::LEFT : {
					if ((getXSpeed() == 0.0f) && (_currentSpeed >= 0.0f))
						_currentSpeed = 0.0f;
					setXSpeed(-_currentSpeed);
					break;
				}
				case Direction::RIGHT : {
					if ((getXSpeed() == 0.0f) && (_currentSpeed >= 0.0f))
						_currentSpeed = 0.0f;
					setXSpeed(_currentSpeed);
					break;
				}
			}
			break;
		}
	}

	if (!_rotation)
		switch (_inertiaDirection) {
		    case Direction::UP :
		    case Direction::DOWN :
				setYSpeed(setBrakingSpeedY(getYSpeed(), dt));
			    break;

		    case Direction::LEFT :
		    case Direction::RIGHT :
				setXSpeed(setBrakingSpeedX(getXSpeed(), dt));
			    break;
		}

	// Приведение текущей скорости танка к нулю
	if ((_currentSpeed > 0.0f) && (_currentSpeed <= (_brakingSpeed * dt)))
		_currentSpeed = 0.0f;
	else if (_currentSpeed > 0.0f)
		_currentSpeed -= _brakingSpeed * dt;	
	
	if ((_currentSpeed < 0.0f) && (_currentSpeed >= -(_brakingSpeed * dt)))
		_currentSpeed = 0.0f;
	else if (_currentSpeed < 0.0f)
		_currentSpeed += _brakingSpeed * dt;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Объезд бокового препятствия на пути танка

bool Tank::bypassObstruction() {
	// Инициализация трёх габаритных точек(зон):

	// верхняя или левая зона передней части танка
	CoordPoint firstSidewardPoint;
	// нижняя или правая зона передней части танка
	CoordPoint secondSidewardPoint;
	// центральная зона передней части танка
	CoordPoint middlePoint;

	float size = 0.0f;

	// Изначальный размер берётся по меньшей стороне танка (по переду танка)
	if (getHeight() < getWidth())
	    size = getHeight();
	else
		size = getWidth();

	// Габариты зон квадратные, берутся за треть меньшей стороны танка
	firstSidewardPoint.width = size / 3.0f;
	firstSidewardPoint.height = size / 3.0f;
	secondSidewardPoint.width = size / 3.0f;
	secondSidewardPoint.height = size / 3.0f;
	middlePoint.width = size / 3.0f;
	middlePoint.height = size / 3.0f;

	// В зависимости от направления, точки устанавливаются точно на лобовую часть танка
	// с небольшим смещением в сторону орудия
	switch (getDirection()) {
	    case Direction::UP : {
			firstSidewardPoint.coordX = getX();
			firstSidewardPoint.coordY = getY() - 0.1f;
			secondSidewardPoint.coordX = getX() + getWidth() - secondSidewardPoint.width;
			secondSidewardPoint.coordY = getY() - 0.1f;
			middlePoint.coordX = getX() + middlePoint.width;
			middlePoint.coordY = getY() - 0.1f;
			break;
	    }
	    case Direction::RIGHT : {
			firstSidewardPoint.coordX = getX() + getWidth() - firstSidewardPoint.width + 0.1f;
			firstSidewardPoint.coordY = getY();
			secondSidewardPoint.coordX = getX() + getWidth() - secondSidewardPoint.width + 0.1f;
			secondSidewardPoint.coordY = getY() + getHeight() - secondSidewardPoint.height;
			middlePoint.coordX = getX() + getWidth() - middlePoint.width + 0.1f;
			middlePoint.coordY = getY() + middlePoint.height;
			break;
	    }
	    case Direction::DOWN : {
			firstSidewardPoint.coordX = getX();
			firstSidewardPoint.coordY = getY() + getHeight() - firstSidewardPoint.height + 0.1f;
			secondSidewardPoint.coordX = getX() + getWidth() - secondSidewardPoint.width;
			secondSidewardPoint.coordY = getY() + getHeight() - secondSidewardPoint.height + 0.1f;
			middlePoint.coordX = getX() + middlePoint.width;
			middlePoint.coordY = getY() + getHeight() - middlePoint.height + 0.1f;
			break;
	    }
	    case Direction::LEFT : {
			firstSidewardPoint.coordX = getX() - 0.1f;
			firstSidewardPoint.coordY = getY();
			secondSidewardPoint.coordX = getX() - 0.1f;
			secondSidewardPoint.coordY = getY() + getHeight() - secondSidewardPoint.height;
			middlePoint.coordX = getX() - 0.1f;
			middlePoint.coordY = getY() + middlePoint.height;
			break;
	    }
	}

	bool firstPointIntersects = bool(getGame().checkIntersects(firstSidewardPoint.coordX, firstSidewardPoint.coordY,
		                                               firstSidewardPoint.width, firstSidewardPoint.height,
		                                               this));
	bool secondPointIntersects = bool(getGame().checkIntersects(secondSidewardPoint.coordX, secondSidewardPoint.coordY,
		                                                secondSidewardPoint.width, secondSidewardPoint.height,
		                                                this));
	bool middlePointIntersects = bool(getGame().checkIntersects(middlePoint.coordX, middlePoint.coordY,
		                                                middlePoint.width, middlePoint.height,
		                                                this));

	if (!firstPointIntersects && !secondPointIntersects)
		return false;
	
	// Если непосредственно под орудием есть объект-помеха, то смещать танк бессмысленно
	if (middlePointIntersects)
		return false;

	// Если на обоих передних катках регистрируется пересечение с другим объектом,
	// то танк смещать бессмысленно (проезд либо узкий, либо его нет)
	if (firstPointIntersects && secondPointIntersects)
		return false;

	float shiftSpeed = 0.6f;

	// Произвести движение вбок от препятствия
	if (firstPointIntersects) {
		switch (getDirection()) {
		    case Direction::UP : {
				if (getXSpeed() < shiftSpeed)
					setXSpeed(shiftSpeed);
				break;
		    }
			case Direction::RIGHT : {
				if (getYSpeed() < shiftSpeed)
					setYSpeed(shiftSpeed);
				break;
			}
			case Direction::DOWN : {
				if (getXSpeed() < shiftSpeed)
					setXSpeed(shiftSpeed);
				break;
			}
			case Direction::LEFT : {
				if (getYSpeed() < shiftSpeed)
					setYSpeed(shiftSpeed);
				break;
			}
		}
		return true;
	}

	if (secondPointIntersects) {
		switch (getDirection()) {
		    case Direction::UP : {
			    if (getXSpeed() > -shiftSpeed)
			    	setXSpeed(-shiftSpeed);
			    break;
		    }
		    case Direction::RIGHT : {
			    if (getYSpeed() > -shiftSpeed)
			    	setYSpeed(-shiftSpeed);
			    break;
		    }
		    case Direction::DOWN : {
			    if (getXSpeed() > -shiftSpeed)
			    	setXSpeed(-shiftSpeed);
			    break;
		    }
		    case Direction::LEFT : {
			    if (getYSpeed() > -shiftSpeed)
			    	setYSpeed(-shiftSpeed);
			    break;
		    }
		}
		return true;
	}

	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Вращение спрайта танка

bool Tank::rotation(float dt) {
	short targetRotation = 90 * (int)getDirection();
	if (_rotationAngle == targetRotation)
		return false;

	_currentRotationTime += dt;
	float anglePerSecond = 90.0f / _rotationTime;
	short coefficientTime = 1;
	
	switch (_oldDirection) {
	    case Direction::UP : {
			switch (getDirection()) {
			    case Direction::LEFT :
				    _rotationAngle -= anglePerSecond * dt;
				    break;

				case Direction::DOWN :
					coefficientTime = 2;
				case Direction::RIGHT :
					_rotationAngle += anglePerSecond * dt;
					break;
			}
			_spriteEntity->setRotation(_rotationAngle);
			break;
	    }

		case Direction::RIGHT : {
			switch (getDirection()) {
			    case Direction::UP :
					_rotationAngle -= anglePerSecond * dt;
					break;
				
				case Direction::LEFT :
					coefficientTime = 2;
				case Direction::DOWN :
					_rotationAngle += anglePerSecond * dt;
					break;
			}
			_spriteEntity->setRotation(_rotationAngle);
			break;
		}

		case Direction::DOWN : {
			switch (getDirection()) {
			    case Direction::RIGHT :
					_rotationAngle -= anglePerSecond * dt;
				    break;

				case Direction::UP :
					coefficientTime = 2;
				case Direction::LEFT :
					_rotationAngle += anglePerSecond * dt;
					break;
			}
			_spriteEntity->setRotation(_rotationAngle);
			break;
		}

		case Direction::LEFT : {
			switch (getDirection()) {
			    case Direction::DOWN :
					_rotationAngle -= anglePerSecond * dt;
					break;

				case Direction::RIGHT:
					coefficientTime = 2;
				case Direction::UP :
					_rotationAngle += anglePerSecond * dt;
					break;
			}
			_spriteEntity->setRotation(_rotationAngle);
			break;
		}
	}

	if (_currentRotationTime >= _rotationTime * coefficientTime) {
		_rotationAngle = (float)targetRotation;
		_spriteEntity->setRotation(_rotationAngle);
		_currentRotationTime = 0.0f;
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Блок методов, реализующих выстрел танка

void Tank::fire() {
	if ((_fireCooldownTime > 0) || _rotation)
		return;

	_fireCooldownTime = TANK_FIRE_COOLDOWN_TIME;

	float bulletPositionX = 0.0f;
	float bulletPositionY = 0.0f;
	float bulletSpeedX = 0.0f;
	float bulletSpeedY = 0.0f;

	calculateFrontCellPosition(bulletPositionX, bulletPositionY);

	switch (getDirection()) {
	    case Direction::LEFT :
			bulletSpeedX = -BULLET_SPEED;
            bulletPositionY -= (STANDART_BULLET_WIDTH / 2.0f);
            bulletPositionX -= STANDART_BULLET_HEIGHT;
		    break;

		case Direction::RIGHT :
			bulletSpeedX = BULLET_SPEED;
            bulletPositionY -= (STANDART_BULLET_WIDTH / 2.0f);
			break;

		case Direction::UP :
			bulletSpeedY = -BULLET_SPEED;
            bulletPositionX -= (STANDART_BULLET_WIDTH / 2.0f);
            bulletPositionY -= STANDART_BULLET_HEIGHT;
			break;

		case Direction::DOWN :
			bulletSpeedY = BULLET_SPEED;
            bulletPositionX -= (STANDART_BULLET_WIDTH / 2.0f);
			break;
	}
	std::unique_ptr<GameObject>& object = 
        getGame().createObject(GameObjectType::BULLET,
                               bulletPositionX, bulletPositionY);
    if (object) {
        Bullet& bullet = dynamic_cast<Bullet&>(*object);
        bullet.setOwnerType(this->getType());
        bullet.setXSpeed(bulletSpeedX);
        bullet.setYSpeed(bulletSpeedY);
        bullet.setDirection(this->getDirection());
        bullet.setTextureRect(BULLET_IMAGE);
    }
	//Отдача от выстрела
	_currentSpeed -= _speedup / 6.0f;
}


void Tank::calculateFrontCellPosition(float& x, float& y) {
    // Дополнительная длина до дула танка
	float distanceToGunpoint = 0.0f;

	switch (getType()) {
	    case GameObjectType::TANK_FIRST_PLAYER :
		case GameObjectType::TANK_SECOND_PLAYER :
		case GameObjectType::TANK_ENEMY :
			distanceToGunpoint = 0.3f;
			break;
	}
	switch (getDirection()) {
	    // Расчёты строятся на том, что координатная точка танка
		// находится в верхнем левом углу относительно его габаритов
	    case Direction::UP :
		    x = getX() + (getWidth() / 2.0f);
		    y = getY() - distanceToGunpoint;
		    break;

		case Direction::RIGHT :
			x = getX() + getWidth() + distanceToGunpoint;
			y = getY() + (getHeight() / 2.0f);
			break;
		
		case Direction::DOWN :
			x = getX() + (getWidth() / 2.0f);
			y = getY() + getHeight() + distanceToGunpoint;
			break;
		
	    case Direction::LEFT :
		    x = getX() - distanceToGunpoint;
		    y = getY() + (getHeight() / 2.0f);
		    break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Блок методов торможения по инерции

float Tank::setBrakingSpeedX(float xSpeed, float dt) {
	if ((xSpeed > 0.0f) && (xSpeed <= (_brakingSpeed * dt)))
		return 0.0f;
	else if (xSpeed > 0.0f)
		return xSpeed - _brakingSpeed * dt;

	if ((xSpeed < 0.0f) && (xSpeed >= -(_brakingSpeed * dt)))
		return 0.0f;
	else if (xSpeed < 0.0f)
		return xSpeed + _brakingSpeed * dt;

	return 0.0f;
}

float Tank::setBrakingSpeedY(float ySpeed, float dt) {
    if ((ySpeed > 0.0f) && (ySpeed <= (_brakingSpeed * dt)))
		return 0.0f;
	else if (ySpeed > 0.0f)
	    return ySpeed - _brakingSpeed * dt;

    if ((ySpeed < 0.0f) && (ySpeed >= -(_brakingSpeed * dt)))
		return 0.0f;
	else if (ySpeed < 0.0f)
		return ySpeed + _brakingSpeed * dt;
	    
	return 0.0f;
}