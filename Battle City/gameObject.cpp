#include "GameObject.h"
#include "Game.h"
#include "CoordPoint.h"
#include "Utils.h"


GameObject::GameObject() {
	_type = GameObjectType::NONE;

	_direction = Direction::UP;
}

void GameObject::render(sf::RenderWindow* rw) {
    // ���� ������� �������� ���� ������
	if (_spriteTerrain || _spriteEntity) {
		if (_spriteTerrain) {
            // ��������� ������� �� ������� ����.
            // ��������������, ��� ����� ������� ������ �� ����� ����� ��������
			_spriteTerrain->setPosition(getX() * PIXELS_PER_CELL
                                        + (getWidth() * PIXELS_PER_CELL) / 2.0f,
				                        getY() * PIXELS_PER_CELL
                                        + (getHeight() * PIXELS_PER_CELL) / 2.0f);
			rw->draw(*_spriteTerrain);
		}
		else if (_spriteEntity) {
			_spriteEntity->setPosition(getX() * PIXELS_PER_CELL
                                       + (getWidth() * PIXELS_PER_CELL) / 2.0f,
				                       getY() * PIXELS_PER_CELL
                                       + (getHeight() * PIXELS_PER_CELL) / 2.0f);
			rw->draw(*_spriteEntity);
		}
	}
}

void GameObject::update(float dt) {
	switch (getType()) {
	    case GameObjectType::TANK_ENEMY :
	    case GameObjectType::TANK_FIRST_PLAYER :
		case GameObjectType::TANK_SECOND_PLAYER : {
            // ���� � ������ ������ ������ �� �������� ��������� ������ ������ ������
			if (!getInBypass())
                // �� ���������� �������� �� ��������
			    _isSticking = _game->checkIntersects(getX(), getY(),
                                                     getWidth(), getHeight(), this);
            // ���� ������ ������� � ������ �������
			if (getIsSticking())
				escapeSticking();
		}
		
        case GameObjectType::BULLET : {
            float oldX = getX();
            float oldY = getY();        

            float newX = getX() + getXSpeed() * dt; // ����� ������� �� �����������
            float newY = getY() + getYSpeed() * dt; // ����� ������� �� ���������
            // ����� ���������� ������������� �� ������� �������� � ������ �������,
            // ����������� �� ���������� ���� ���������� ������� ������
         
            // ���� ���� �� ���� ��������� �� ��������� � ����������,
            // �� ������ ������������
            if ((oldX != newX || oldY != newY) && !_isSticking && !_inBypass) {
                _game->moveObjectTo(this, newX, newY);
            } else {
                setX(newX);
                setY(newY);
            }
        }
        break;
	}
}

void GameObject::escapeSticking() {
    CoordPoint topLeftPoint;
    CoordPoint topRightPoint;
    CoordPoint bottomRightPoint;
    CoordPoint bottomLeftPoint;
		
    // �������� ���������� �������� ������� �� ������ ������ �����
    topLeftPoint.setCoordPoint    (this, typeCoordPoint::TOP_LEFT);
    topRightPoint.setCoordPoint   (this, typeCoordPoint::TOP_RIGHT);
    bottomRightPoint.setCoordPoint(this, typeCoordPoint::BOTTOM_RIGHT);
    bottomLeftPoint.setCoordPoint (this, typeCoordPoint::BOTTOM_LEFT);
		
    // �������� �� �������� � ������ �������� ������ �������� �������� �������
    /////////////////////////
    bool topLeftIntersect = _game->checkIntersects(
        topLeftPoint.coordX, topLeftPoint.coordY,
        topLeftPoint.width, topLeftPoint.height, this);

    bool topRightIntersect = _game->checkIntersects(
        topRightPoint.coordX, topRightPoint.coordY,
        topRightPoint.width, topRightPoint.height, this);

    bool bottomRightIntersect = _game->checkIntersects(
        bottomRightPoint.coordX, bottomRightPoint.coordY,
        bottomRightPoint.width, bottomRightPoint.height, this);

    bool bottomLeftIntersect = _game->checkIntersects(
        bottomLeftPoint.coordX, bottomLeftPoint.coordY,
        bottomLeftPoint.width, bottomLeftPoint.height, this);

    // ��� �������� �������� ������������ ����� ��������
    bool primarySet{false};
    // �������� ������������ ������� �� ������� �������
    const float SHIFT_SPEED{0.6f};

    // ��������� �������� �� ��, ��� ��� �������� ����� �������
    // ��������� ������ ������ ��������
    if (topLeftIntersect && topRightIntersect) {
        if (getYSpeed() < SHIFT_SPEED)
            setYSpeed(SHIFT_SPEED);
        else
            setYSpeed(getYSpeed());
        primarySet = true;
    }
    if (topRightIntersect && bottomRightIntersect) {
        if (getXSpeed() > -SHIFT_SPEED)
            setXSpeed(-SHIFT_SPEED);
        else
            setXSpeed(getXSpeed());
        primarySet = true;
    }
    if (bottomRightIntersect && bottomLeftIntersect) {
        if (getYSpeed() > -SHIFT_SPEED)
            setYSpeed(-SHIFT_SPEED);
        else
            setYSpeed(getYSpeed());
        primarySet = true;
    }
    if (bottomLeftIntersect && topLeftIntersect) {
        if (getXSpeed() < SHIFT_SPEED)
            setXSpeed(SHIFT_SPEED);
        else
            setXSpeed(getXSpeed());
        primarySet = true;
    }
    if (primarySet)
        return;

    // ���� ������ ���� �������� ����� ��������
    if (topLeftIntersect)
        switch (getDirection()) {
            case Direction::UP :
            case Direction::RIGHT : {
                if (getXSpeed() < SHIFT_SPEED)
                    setXSpeed(SHIFT_SPEED);
                else
                    setXSpeed(getXSpeed());
                break;
            }
            case Direction::DOWN :
            case Direction::LEFT : {
                if (getYSpeed() < SHIFT_SPEED)
                    setYSpeed(SHIFT_SPEED);
                else
                    setYSpeed(getYSpeed());
                break;
            }
        }
    else if (topRightIntersect)
        switch (getDirection()) {
            case Direction::UP :
            case Direction::LEFT : {
                if (getXSpeed() > -SHIFT_SPEED)
                    setXSpeed(-SHIFT_SPEED);
                else
                    setXSpeed(getXSpeed());
                break;
            }
            case Direction::DOWN :
            case Direction::RIGHT : {
                if (getYSpeed() < SHIFT_SPEED)
                    setYSpeed(SHIFT_SPEED);
                else
                    setYSpeed(getYSpeed());
                break;
            }
        }
    else if (bottomRightIntersect)
        switch (getDirection()) {
            case Direction::LEFT :
            case Direction::DOWN : {
                if (getXSpeed() > -SHIFT_SPEED)
                    setXSpeed(-SHIFT_SPEED);
                else
                    setXSpeed(getXSpeed());
                break;
            }
            case Direction::UP :
            case Direction::RIGHT : {
                if (getYSpeed() > -SHIFT_SPEED)
                    setYSpeed(-SHIFT_SPEED);
                else
                    setYSpeed(getYSpeed());
                break;
            }
        }
    else if (bottomLeftIntersect)
        switch (getDirection()) {
            case Direction::RIGHT :
            case Direction::DOWN : {
                if (getXSpeed() < SHIFT_SPEED)
                    setXSpeed(SHIFT_SPEED);
                else
                    setXSpeed(getXSpeed());
                break;
            }
            case Direction::LEFT :
            case Direction::UP : {
                if (getYSpeed() > -SHIFT_SPEED)
                    setYSpeed(-SHIFT_SPEED);
                else
                    setYSpeed(getYSpeed());
                break;
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