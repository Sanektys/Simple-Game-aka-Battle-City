#include "Tank.h"
#include "Bullet.h"
#include "GameObjectType.h"
#include "Game.h"
#include "CoordPoint.h"


Tank::Tank(const class Game& game) : GameObject(game) {
    setGroup(GameObjectGroup::ENTITY);

    setWidth(level::tank::WIDTH);
    setHeight(level::tank::HEIGHT);
    _offset = (getHeight() - getWidth()) / 2.0f;

    _rotationAngle = 90.0f * (float)getDirection();

    _prevX = getX();
    _prevY = getY();

    _spriteEntity.reset(new sf::Sprite());
    _spriteEntity->setTexture(*(level::ATLAS_ENTITY));
    _spriteEntity->setOrigin(level::tank::PIXELS_WIDTH / 2.0f,
                             level::tank::PIXELS_HEIGHT / 2.0f 
                             + level::tank::PIXELS_GUN_LENGTH);
}

///////////////////////////////////////////////////////////
// ���������� ��������� ����� ������ ������� ����

void Tank::update(float dt) {
    // ��� ����� ����������� �������� �� 90 �������� ������������ ���������� ���������
    // ����� ������ � ��� ������������ ������. ����� ����������� ������� �������� �������
    if (_oldDirection != getDirection() && _rotation == false) {
        _rotation = true;

        switch (getDirection()) {
            // ���� ����������� ���������� �� 180 ��������,
            // �� ����������� ���������� ��������� �� �����
            case Direction::UP :
                if (_oldDirection == Direction::DOWN)
                    break;
            case Direction::DOWN : {
                if (_oldDirection == Direction::UP)
                    break;

                float newY = getY() - _offset;
                float newX = getX() + _offset;
                float newWidth = level::tank::WIDTH;
                float newHeight = level::tank::HEIGHT;

                // ����������� ����� ��������� �����, ����� �� ��� ������������
                // � �������� �������� 2�2 �� � � � �������� �����������
                float uncollisionPart = level::tank::WIDTH / 10.0f;

                bool topIntersects = bool(
                    getGame().checkIntersects(newX + uncollisionPart, newY, 
                                              newWidth - uncollisionPart * 2.0f, _offset, this));
                bool bottomIntersects = bool(
                    getGame().checkIntersects(newX + uncollisionPart, getY() + getHeight(),
                                              newWidth - uncollisionPart * 2.0f, _offset, this));

                // ���� ���� ����� ������ � �����,
                // � ��� ����� �������� �� ���������� �� ������� �����,
                // �� �������� ����� ����������� � ������ ��������
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
                float newWidth = level::tank::HEIGHT;
                float newHeight = level::tank::WIDTH;

                float uncollisionPart = level::tank::WIDTH / 10.0f;

                bool rightIntersects = bool(
                    getGame().checkIntersects(getX() + getWidth(),newY + uncollisionPart,
                                              _offset, newHeight - uncollisionPart * 2.0f, this));
                bool leftIntersects = bool(
                    getGame().checkIntersects(newX, newY + uncollisionPart, _offset,
                                              newHeight - uncollisionPart * 2.0f, this));

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

            default :
                break;
        }
    }

    if (_rotation)
        if (!rotation(dt)) {
            _rotation = false;
            _currentSpeed = 0.0f;
            _oldDirection = getDirection();
        }

    if (_fireCooldownTime > 0)
        _fireCooldownTime -= dt;

    // ���������� �������� ������
    if (_prevX != getX() || _prevY != getY())
        _currentTrackShift += std::abs(_currentSpeed) * dt;
    _prevX = getX();
    _prevY = getY();

    GameObject::update(dt);
}

///////////////////////////////////////////////////////////
// ��������� �����

void Tank::renderTracksMoving() {
    // ����������, ����� ����������� �������� �������� ������ ������
    const float TRACK_SHIFT_INTERVAL = 0.08f;

    // ���� ���������� ��������
    if (_currentTrackShift >= TRACK_SHIFT_INTERVAL) {
        _currentTrackShift = 0.0f;
        
        sf::IntRect textureRect = _spriteEntity->getTextureRect();
        // ������� �� �������� ������ �� ����������� � ������
        if (0 == textureRect.left)
            textureRect.left = level::tank::PIXELS_WIDTH;
        else
            textureRect.left = 0;

        _spriteEntity->setTextureRect(textureRect);
    }
}

void Tank::render(sf::RenderWindow* rw) {
    renderTracksMoving();

    /////////////////////////////////////
    /*using level::PIXELS_PER_CELL;
    float x{0.0f};
    float y{0.0f};
    calculateFrontCellPosition(x, y);
    sf::RectangleShape pick;
    pick.setFillColor(sf::Color(255, 0, 0));
    switch (getDirection()) {
        case Direction::UP : 
            pick.setSize(sf::Vector2f(1.0f, 100.0f));
            pick.setPosition(x * PIXELS_PER_CELL - 0.5f, y * PIXELS_PER_CELL - 100.0f);
            break;
        case Direction::DOWN :
            pick.setSize(sf::Vector2f(1.0f, 100.0f));
            pick.setPosition(x * PIXELS_PER_CELL - 0.5f, y * PIXELS_PER_CELL);
            break;
        case Direction::RIGHT :
            pick.setSize(sf::Vector2f(100.0f, 1.0f));
            pick.setPosition(x * PIXELS_PER_CELL, y * PIXELS_PER_CELL - 0.5f);
            break;
        case Direction::LEFT :
            pick.setSize(sf::Vector2f(100.0f, 1.0f));
            pick.setPosition(x * PIXELS_PER_CELL - 100.0f, y * PIXELS_PER_CELL - 0.5f);
            break;
    }
    rw->draw(pick);*/
    /////////////////////////////////////

    GameObject::render(rw);
}

///////////////////////////////////////////////////////////
// ������� �� ��������� ������� �������� ����� �� ������ ������� �����

void Tank::move(enum Direction direction, float dt) {
    if (_rotation)
        direction = Direction::NONE;

    // ������������� ����������� �� ��������������� ��� �������� ��� �����,
    // ��� ���������� ������ ��� ���������� ������������� � �������� ������ �������
    if (direction != Direction::NONE)
        setDirection(direction);

    // !_rotation ����� ��������, ��� �������� ������ ��� �������� ����� �����
    // ����������� ����, �� _rotation ��� �� ���������� � true
    if (!_rotation)
        if (getDirection() != _oldDirection) {
            // ��� ����� ���������� ��������, ������� ���������������
            // �� �����������, ��������������� ������������� � ��������
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
                default :
                    break;
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
            // ��� inBypass = true �������� �� �����������
            // ������ �������� ������ ����� ��������������
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
        // ��� ������������� ����������� �� ������������ ���������� ��������.
        // �������� �� �������� �����������, ���� ���� ��������� ���������������,
        // ���� ������������� �� ������������ ������� �������, ���� ���� �������
        case Direction::NONE : {
            setInBypass(false);

            // ��� �������� ����� ���� ���� �������� ��������� ���� ������� ��
            // �������� �����������, � ��� �������� ������ ����� ������� ��������
            _currentSpeed = setBrakingSpeed(_currentSpeed, dt);
            switch (_oldDirection) {
                case Direction::UP : 
                    // ���� (��� ��������) �� ������� ������� ���� "��������",
                    // �� ��� ������������� �������� ������������ ���
                    // ����������� ������� ������ �� �������������� ������������
                    // ��� ��������� ��������� �����
                    if (getYSpeed() == 0.0f && _currentSpeed >= 0.0f)
                        _currentSpeed = 0.0f;
                    setYSpeed(-_currentSpeed);
                    break;

                case Direction::DOWN :
                    if (getYSpeed() == 0.0f && _currentSpeed >= 0.0f)
                        _currentSpeed = 0.0f;
                    setYSpeed(_currentSpeed);
                    break;

                case Direction::RIGHT :
                    if (getXSpeed() == 0.0f && _currentSpeed >= 0.0f)
                        _currentSpeed = 0.0f;
                    setXSpeed(_currentSpeed);
                    break;

                case Direction::LEFT :
                    if (getXSpeed() == 0.0f && _currentSpeed >= 0.0f)
                        _currentSpeed = 0.0f;
                    setXSpeed(-_currentSpeed);
                    break;

                default :
                    break;
            }
            break;
        }
        default :
            break;
    }

    // ���������� ���������� ������������ �������� �� �������� �����������,
    // ����� ������� ��� ����������
    if (!_rotation)
        switch (_inertiaDirection) {
            case Direction::UP :
            case Direction::DOWN :
                setYSpeed(setBrakingSpeed(getYSpeed(), dt));
                break;

            case Direction::LEFT :
            case Direction::RIGHT :
                setXSpeed(setBrakingSpeed(getXSpeed(), dt));
                break;

            default:
                break;
        }
}

float Tank::setBrakingSpeed(float speed, float dt) {
    float reducedSpeed = std::abs(speed);
    if (reducedSpeed >= 0.0f && reducedSpeed <= _brakingSpeed * dt)
        return 0.0f;
    else if (reducedSpeed > 0.0f)
        return speed + (speed > 0.0f ?
                        -(_brakingSpeed * dt) : +(_brakingSpeed * dt));
    return 0.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// ������ �������� ����������� �� ���� �����

bool Tank::bypassObstruction() {
    // ������������� ��� ���������� �����(���):

    // ������� ��� ����� ���� �������� ����� �����
    CoordPoint firstSidewardPoint;
    // ������ ��� ������ ���� �������� ����� �����
    CoordPoint secondSidewardPoint;
    // ����������� ���� �������� ����� �����
    CoordPoint middlePoint;

    float size = 0.0f;

    // ����������� ������ ������ �� ������� ������� ����� (�� ������ �����)
    if (getHeight() < getWidth())
        size = getHeight();
    else
        size = getWidth();

    // �������� ��� ����������, ������� �� ����� ������� ������� �����
    firstSidewardPoint.width = size / 3.0f;
    firstSidewardPoint.height = size / 3.0f;
    secondSidewardPoint.width = size / 3.0f;
    secondSidewardPoint.height = size / 3.0f;
    middlePoint.width = size / 3.0f;
    middlePoint.height = size / 3.0f;

    // � ����������� �� �����������, ����� ��������������� ����� �� ������� ����� �����
    // � ��������� ��������� � ������� ������
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
        default :
            break;
    }

    bool firstPointIntersects = bool(
        getGame().checkIntersects(firstSidewardPoint.coordX, firstSidewardPoint.coordY,
                                  firstSidewardPoint.width, firstSidewardPoint.height,
                                  this));
    bool secondPointIntersects = bool(
        getGame().checkIntersects(secondSidewardPoint.coordX, secondSidewardPoint.coordY,
                                  secondSidewardPoint.width, secondSidewardPoint.height,
                                  this));
    bool middlePointIntersects = bool(
        getGame().checkIntersects(middlePoint.coordX, middlePoint.coordY,
                                  middlePoint.width, middlePoint.height,
                                  this));

    if (!firstPointIntersects && !secondPointIntersects)
        return false;

    // ���� ��������������� ��� ������� ���� ������-������,
    // �� ������� ���� ������������
    if (middlePointIntersects)
        return false;

    // ���� �� ����� �������� ������ �������������� ����������� � ������ ��������,
    // �� ���� ������� ������������ (������ ���� �����, ���� ��� ���)
    if (firstPointIntersects && secondPointIntersects)
        return false;

    float shiftSpeed = 0.6f;

    // ���������� �������� ���� �� �����������
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
            default :
                break;
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
            default :
                break;
        }
        return true;
    }

    return false;
}


///////////////////////////////////////////////////////////
// �������� ������� �����

bool Tank::rotation(float dt) {
    // RIGHT = 90, DOWN = 180, LEFT = 270, UP = 360
    int targetRotation = 90 * (int)getDirection();

    float anglePerSecond = 90.0f / _rotationTime;

    // �� ����� ������� ����� �������� �����, � ����� ����������� ����������� �������
    float deltaRotation = float(targetRotation) - _rotationAngle;
    // ���� ������� ����� ������� � ������� ����� ������ 180 ��������,
    // ��, ���� ���������� ����� ����������� � UP(360) �� RIGHT(90) ��� ��������
    if (std::abs(deltaRotation) > 180.0f) {
        if (_rotationAngle >= 360.0f)  // ���� ������� ���� ��� �� 360�� ��� ����,
            _rotationAngle -= 360.0f;  // �� �������� � ������ ��� RIGHT(90)
        else if (_rotationAngle >= 0.0f)  // ���� ������� ���� � ������, ��
            _rotationAngle += 360.0f;     // �������� ������ ��� UP(360)

        deltaRotation = -deltaRotation; //����� ����������� ��������
    }
    if (deltaRotation >= 0.0f)
        _rotationClockwise = true;
    else
        _rotationClockwise = false;

    _rotationAngle += _rotationClockwise ? +(anglePerSecond * dt)
                                         : -(anglePerSecond * dt);
    _spriteEntity->setRotation(_rotationAngle);

    // ������ ���������� �� �������� ���� ��� ��� ���������� ��� "������" �� ����
    if (_rotationClockwise ? _rotationAngle >= targetRotation
                           : _rotationAngle <= targetRotation) {
        _rotationAngle = (float)targetRotation;
        _spriteEntity->setRotation(_rotationAngle);
        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////
// ���� �������, ����������� ������� �����

void Tank::fire() {
    if (_fireCooldownTime > 0 || _rotation)
        return;

    switch (getType()) {
        case GameObjectType::TANK_ENEMY :
            _fireCooldownTime = level::tank::enemy::basic::FIRE_COOLDOWN_TIME;
            break;
            
        case GameObjectType::TANK_FIRST_PLAYER :
        case GameObjectType::TANK_SECOND_PLAYER :
            _fireCooldownTime = level::tank::player::FIRE_COOLDOWN_TIME;
            break;

        default :
            break;
    }

    float bulletPositionX = 0.0f;
    float bulletPositionY = 0.0f;
    float bulletSpeedX = 0.0f;
    float bulletSpeedY = 0.0f;

    calculateFrontCellPosition(bulletPositionX, bulletPositionY);

    // ��������� �������� ����� ������� � ��� ��������� ������������
    // ����� �������� � ���� ������ �����
    switch (getDirection()) {
        case Direction::LEFT :
            bulletSpeedX = -level::bullet::basic::SPEED;
            bulletPositionY -= (level::bullet::basic::HEIGHT / 2.0f);
            bulletPositionX -= level::bullet::basic::WIDTH;
            break;

        case Direction::RIGHT :
            bulletSpeedX = level::bullet::basic::SPEED;
            bulletPositionY -= (level::bullet::basic::HEIGHT / 2.0f);
            break;

        case Direction::UP :
            bulletSpeedY = -level::bullet::basic::SPEED;
            bulletPositionX -= (level::bullet::basic::WIDTH / 2.0f);
            bulletPositionY -= level::bullet::basic::HEIGHT;
            break;

        case Direction::DOWN :
            bulletSpeedY = level::bullet::basic::SPEED;
            bulletPositionX -= (level::bullet::basic::WIDTH / 2.0f);
            break;

        default :
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
        bullet.setTextureRect(level::bullet::basic::IMAGE); // ��� �������� �������
    }

    //������ �� ��������
    _currentSpeed -= getMaxSpeed() / 2.0f;
}


void Tank::calculateFrontCellPosition(float& x, float& y) {
    // �������������� ����� �� ���� �����
    float distanceToGunpoint = 0.0f;

    switch (getType()) {
        case GameObjectType::TANK_FIRST_PLAYER :
        case GameObjectType::TANK_SECOND_PLAYER :
        case GameObjectType::TANK_ENEMY :
            distanceToGunpoint = 0.3f;
            break;
        default :
            break;
    }
    switch (getDirection()) {
        // ������� �������� �� ���, ��� ������������ ����� �����
        // ��������� � ������� ����� ���� ������������ ��� ���������
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

        default :
            break;
    }
}