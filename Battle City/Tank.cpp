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

    _spriteEntity.reset(new sf::Sprite());
    _spriteEntity->setTexture(*(level::ATLAS_ENTITY));
    _spriteEntity->setOrigin(level::tank::PIXELS_WIDTH / 2.0f,
                             level::tank::PIXELS_HEIGHT / 2.0f 
                             + level::tank::PIXELS_GUN_LENGTH);

    sf::Color pinColor(sf::Color::White);  // Цвет маркера танка по умолчанию
    pinColor.a = 200;
    _circlePinOnMap.setFillColor(pinColor);
    _circlePinOnMap.setPointCount(16);
    _circlePinOnMap.setRadius(level::tank::WIDTH * level::PIXELS_PER_CELL / 2.0F);
}

///////////////////////////////////////////////////////////
// Обновление состояния танка каждый игровой такт

void Tank::update(float dt) {
    // Проверить вращение
    rotation(dt);

    if (_rotation)  // Пока спрайт полностью не повернётся будет true
        if (!spriteRotation(dt))
            _rotation = false;

    if (_fireCooldownTime > 0)
        _fireCooldownTime -= dt;

    GameObject::update(dt);

    // Обновление смещения траков
    if (getXSpeed() != 0.0f || getYSpeed() != 0.0f)
        _currentTrackShift += std::abs(_currentSpeed) * dt;
}

///////////////////////////////////////////////////////////
// Отрисовка танка

void Tank::render(sf::RenderWindow* rw) {
    renderTracksMoving();

    GameObject::render(rw);
}

void Tank::mapRender(sf::RenderTexture* rt) {
    _circlePinOnMap.setPosition(getX() * level::PIXELS_PER_CELL,
                                getY() * level::PIXELS_PER_CELL);
    rt->draw(_circlePinOnMap);
}

void Tank::renderTracksMoving() {
    // Расстояние, после прохождения которого меняется спрайт траков
    const float TRACK_SHIFT_INTERVAL = 0.08f;

    // Если расстояние пройдено
    if (_currentTrackShift >= TRACK_SHIFT_INTERVAL) {
        _currentTrackShift = 0.0f;

        sf::IntRect textureRect = _spriteEntity->getTextureRect();
        // Переход на соседний спрайт по горизонтали в атласе
        if (0 == textureRect.left)
            textureRect.left = level::tank::PIXELS_WIDTH;
        else
            textureRect.left = 0;

        _spriteEntity->setTextureRect(textureRect);
    }
}

///////////////////////////////////////////////////////////
// Расчёты по установке текущей скорости и направления танка на каждом игровом такте

void Tank::move(enum Direction direction, float dt) {
    if (_rotation) {
        if (direction != Direction::NONE)
            setDirection(direction);  // Направление меняется сразу во время поворота
        direction = Direction::NONE;
    }

    // Отсутствующее направление не устанавливается как параметр для танка,
    // оно необходимо только для временного использования в пределах данной функции
    if (direction != Direction::NONE)
        setDirection(direction);

    // Если выше произошла смена направления движения, то меняется
    // направление, по которому рассчитывается инерция
    if (getDirection() != _oldDirection) {
        // Инерция устанавливается по направлению,
        // перпендикулярно направленному к текущему
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
            default:
                break;
        }

        return;  // Сразу произвести выход из функции
    }

    if (getIsSticking())
        direction = Direction::NONE;

    if (direction != Direction::NONE)
        _currentSpeed += _speedup * dt;

    if (_currentSpeed > _maxSpeed)
        _currentSpeed = _maxSpeed;

    switch (direction) {
        case Direction::UP : {
            // При inBypass = true проверка на пересечение
            // других объектов танком будет игнорироваться
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
            // направлению, параллельному новому, а без поворота просто гасит текущую скорость
            _currentSpeed = setBrakingSpeed(_currentSpeed, dt);
            switch (getDirection()) {
                case Direction::UP : 
                    // Если (при повороте) на прошлой позиции танк "буксовал",
                    // то его потенциальная скорость сбрасывается для
                    // недопущения резкого сдвига на освободившееся пространство
                    // при изменении габаритов танка
                    if (getYSpeed() == 0.0f && _currentSpeed >= 0.0f)
                        _currentSpeed = 0.0f;
                    // При повороте берётся уже набранная скорость по этой оси
                    // и присваивается текущей скорости танка, постоянно снижаясь
                    // во время поворота. Так танк сохранит инерцию при развороте на 180
                    if (_rotation)
                        _currentSpeed = setBrakingSpeed(-getYSpeed(), dt);
                    setYSpeed(-_currentSpeed);
                    // Если есть инерция по перпендикулярному направлению,
                    // то проверить, можно ли отключить коллизию для задних катков
                    if (std::abs(getXSpeed()) != 0.0f)
                        setInBypass(bypassObstruction());
                    break;

                case Direction::DOWN :
                    if (getYSpeed() == 0.0f && _currentSpeed >= 0.0f)
                        _currentSpeed = 0.0f;
                    if (_rotation)
                        _currentSpeed = setBrakingSpeed(getYSpeed(), dt);
                    setYSpeed(_currentSpeed);
                    if (std::abs(getXSpeed()) != 0.0f)
                        setInBypass(bypassObstruction());
                    break;

                case Direction::RIGHT :
                    if (getXSpeed() == 0.0f && _currentSpeed >= 0.0f)
                        _currentSpeed = 0.0f;
                    if (_rotation)
                        _currentSpeed = setBrakingSpeed(getXSpeed(), dt);
                    setXSpeed(_currentSpeed);
                    if (std::abs(getYSpeed()) != 0.0f)
                        setInBypass(bypassObstruction());
                    break;

                case Direction::LEFT :
                    if (getXSpeed() == 0.0f && _currentSpeed >= 0.0f)
                        _currentSpeed = 0.0f;
                    if (_rotation)
                        _currentSpeed = setBrakingSpeed(-getXSpeed(), dt);
                    setXSpeed(-_currentSpeed);
                    if (std::abs(getYSpeed()) != 0.0f)
                        setInBypass(bypassObstruction());
                    break;

                default :
                    break;
            }
            break;
        }
        default :
            break;
    }

    // Блок уменьшения инерциальной скорости танка
    // Инерция всегда перпендикулярна текущему направлению движения
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
// Объезд бокового препятствия на пути танка

bool Tank::bypassObstruction() {
    // Инициализация трёх габаритных точек(зон):

    // верхняя или левая зона передней части танка
    CoordPoint firstSidewardPoint;
    // нижняя или правая зона передней части танка
    CoordPoint secondSidewardPoint;
    // центральная зона передней части танка
    CoordPoint middlePoint;

    // Изначальный размер берётся по меньшей стороне танка (по переду танка)
    float size = getHeight() < getWidth() ? getHeight() : getWidth();

    // Габариты зон квадратные, берутся за треть меньшей стороны танка
    firstSidewardPoint.width  = firstSidewardPoint.height  = size / 3.0f;
    secondSidewardPoint.width = secondSidewardPoint.height = size / 3.0f;
    middlePoint.width         =  middlePoint.height        = size / 3.0f;

    // В зависимости от направления, точки устанавливаются точно на лобовую часть танка
    // с небольшим смещением в сторону орудия
    switch (getDirection()) {
        case Direction::UP :
            firstSidewardPoint.coordX  = getX();
            secondSidewardPoint.coordX = getX() + getWidth() - secondSidewardPoint.width;
            middlePoint.coordX         = getX() + middlePoint.width;

            firstSidewardPoint.coordY = secondSidewardPoint.coordY
                = middlePoint.coordY  = getY() - 0.1f;
            break;

        case Direction::RIGHT :
            firstSidewardPoint.coordY  = getY();
            secondSidewardPoint.coordY = getY() + getHeight() - secondSidewardPoint.height;
            middlePoint.coordY         = getY() + middlePoint.height;

            firstSidewardPoint.coordX = secondSidewardPoint.coordX
                = middlePoint.coordX  = getX() + getWidth() - middlePoint.width + 0.1f;
            break;

        case Direction::DOWN :
            firstSidewardPoint.coordX  = getX();
            secondSidewardPoint.coordX = getX() + getWidth() - secondSidewardPoint.width;
            middlePoint.coordX         = getX() + middlePoint.width;

            firstSidewardPoint.coordY = secondSidewardPoint.coordY
                = middlePoint.coordY  = getY() + getHeight() - middlePoint.height + 0.1f;
            break;

        case Direction::LEFT :
            firstSidewardPoint.coordY  = getY();
            secondSidewardPoint.coordY = getY() + getHeight() - secondSidewardPoint.height;
            middlePoint.coordY         = getY() + middlePoint.height;

            firstSidewardPoint.coordX = secondSidewardPoint.coordX
                = middlePoint.coordX  = getX() - 0.1f;
            break;

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

    // Дополнительные две точки на заднюю половину танка, для избежания резкой
    // остановки на дрифте после поворота, от столкновения с препятствием на задний каток
    CoordPoint firstBackPoint, secondBackPoint;
    switch (getDirection()) {
        case Direction::UP :
            firstBackPoint.setCoordPoint (this, TypeCoordPoint::BOTTOM_LEFT);
            firstBackPoint.coordX  -= 0.1f;  // Точки слегка выпирают по бортам танка

            secondBackPoint.setCoordPoint(this, TypeCoordPoint::BOTTOM_RIGHT);
            secondBackPoint.coordX += 0.1f;
            break;

        case Direction::RIGHT :
            firstBackPoint.setCoordPoint (this, TypeCoordPoint::TOP_LEFT);
            firstBackPoint.coordY  -= 0.1f;

            secondBackPoint.setCoordPoint(this, TypeCoordPoint::BOTTOM_LEFT);
            secondBackPoint.coordY += 0.1f;
            break;

        case Direction::DOWN :
            firstBackPoint.setCoordPoint (this, TypeCoordPoint::TOP_LEFT);
            firstBackPoint.coordX  -= 0.1f;

            secondBackPoint.setCoordPoint(this, TypeCoordPoint::TOP_RIGHT);
            secondBackPoint.coordX += 0.1f;
            break;

        case Direction::LEFT :
            firstBackPoint.setCoordPoint (this, TypeCoordPoint::TOP_RIGHT);
            firstBackPoint.coordY  -= 0.1f;

            secondBackPoint.setCoordPoint(this, TypeCoordPoint::BOTTOM_RIGHT);
            secondBackPoint.coordY += 0.1f;
            break;
    }

    bool firstBackIntersects  = bool(
        getGame().checkIntersects(firstBackPoint.coordX, firstBackPoint.coordY,
                                  firstBackPoint.width, firstBackPoint.height,
                                  this));
    bool secondBackIntersects = bool(
        getGame().checkIntersects(secondBackPoint.coordX, secondBackPoint.coordY,
                                  secondBackPoint.width, secondBackPoint.height,
                                  this));

    // Вообще нет коллизии с другими объектами
    if (!firstPointIntersects && !secondPointIntersects
        && !firstBackIntersects && !secondBackIntersects)
        return false;

    // Если непосредственно под орудием есть объект-помеха,
    // то смещать его бессмысленно
    if (middlePointIntersects)
        return false;

    // Если на обоих передних катках регистрируется пересечение с другим объектом,
    // то танк смещать бессмысленно (проезд либо узкий, либо его нет)
    if (firstPointIntersects && secondPointIntersects)
        return false;

    const float SHIFT_SPEED{0.6f};  // Скорость смещения танка от препятствия
    bool inBypass{false};  // Происходит ли движение сквозь препятствие

    // Произвести движение вбок от препятствия
    if (firstPointIntersects) {
        switch (getDirection()) {
            case Direction::UP :
                if (getXSpeed() < SHIFT_SPEED)
                    setXSpeed(SHIFT_SPEED);
                break;

            case Direction::RIGHT :
                if (getYSpeed() < SHIFT_SPEED)
                    setYSpeed(SHIFT_SPEED);
                break;

            case Direction::DOWN :
                if (getXSpeed() < SHIFT_SPEED)
                    setXSpeed(SHIFT_SPEED);
                break;

            case Direction::LEFT :
                if (getYSpeed() < SHIFT_SPEED)
                    setYSpeed(SHIFT_SPEED);
                break;

            default :
                break;
        }
        inBypass = true;
    }

    if (secondPointIntersects) {
        switch (getDirection()) {
            case Direction::UP :
                if (getXSpeed() > -SHIFT_SPEED)
                    setXSpeed(-SHIFT_SPEED);
                break;

            case Direction::RIGHT :
                if (getYSpeed() > -SHIFT_SPEED)
                    setYSpeed(-SHIFT_SPEED);
                break;

            case Direction::DOWN :
                if (getXSpeed() > -SHIFT_SPEED)
                    setXSpeed(-SHIFT_SPEED);
                break;

            case Direction::LEFT :
                if (getYSpeed() > -SHIFT_SPEED)
                    setYSpeed(-SHIFT_SPEED);
                break;

            default :
                break;
        }
        inBypass = true;
    }
    
    // Движение задних катков через препятствие

    bool hasObstacleAhead = middlePointIntersects
                            || (firstPointIntersects && secondPointIntersects);

    // Если обе задние части бортов имеют пересечение с помехами,
    // то не обрабатывать прохождение сквозь эти помехи
    if (firstBackIntersects && secondBackIntersects)
        return inBypass;

    // Если пересечение регистрируется по диагонали,
    // то вообще не производить обход препятствия
    if ((firstBackIntersects && secondPointIntersects)
        || (secondBackIntersects && firstPointIntersects))
        return false;

    // Если есть пересечение с помехами у борта и у переднего катка,
    // то не обрабатывать прохождение сквозь помехи у задних катков
    if ((firstBackIntersects && firstPointIntersects)
        || (secondBackIntersects && secondPointIntersects))
        return inBypass;

    if (firstBackIntersects) {
        switch (getDirection()) {
            case Direction::UP :
                if (hasObstacleAhead) {
                    if (getXSpeed() < SHIFT_SPEED)
                        setXSpeed(SHIFT_SPEED);
                } else {
                    if (getYSpeed() > -SHIFT_SPEED)
                        setYSpeed(-SHIFT_SPEED);
                }
                break;

            case Direction::RIGHT :
                if (hasObstacleAhead) {
                    if (getYSpeed() < SHIFT_SPEED)
                        setYSpeed(SHIFT_SPEED);
                } else {
                    if (getXSpeed() < SHIFT_SPEED)
                        setXSpeed(SHIFT_SPEED);
                }
                break;

            case Direction::DOWN :
                if (hasObstacleAhead) {
                    if (getXSpeed() < SHIFT_SPEED)
                        setXSpeed(SHIFT_SPEED);
                } else {
                    if (getYSpeed() < SHIFT_SPEED)
                        setYSpeed(SHIFT_SPEED);
                }
                break;

            case Direction::LEFT :
                if (hasObstacleAhead) {
                    if (getYSpeed() < SHIFT_SPEED)
                        setYSpeed(SHIFT_SPEED);
                } else {
                    if (getXSpeed() > -SHIFT_SPEED)
                        setXSpeed(-SHIFT_SPEED);
                }
                break;
        }
        inBypass = true;
    }

    if (secondBackIntersects) {
        switch (getDirection()) {
            case Direction::UP :
                if (hasObstacleAhead) {
                    if (getXSpeed() > -SHIFT_SPEED)
                        setXSpeed(-SHIFT_SPEED);
                } else {
                    if (getYSpeed() > -SHIFT_SPEED)
                        setYSpeed(-SHIFT_SPEED);
                }
                break;

            case Direction::RIGHT :
                if (hasObstacleAhead) {
                    if (getYSpeed() > -SHIFT_SPEED)
                        setYSpeed(-SHIFT_SPEED);
                } else {
                    if (getXSpeed() < SHIFT_SPEED)
                        setXSpeed(SHIFT_SPEED);
                }
                break;

            case Direction::DOWN :
                if (hasObstacleAhead) {
                    if (getXSpeed() > -SHIFT_SPEED)
                        setXSpeed(-SHIFT_SPEED);
                } else {
                    if (getYSpeed() < SHIFT_SPEED)
                        setYSpeed(SHIFT_SPEED);
                }
                break;

            case Direction::LEFT :
                if (hasObstacleAhead) {
                    if (getYSpeed() > -SHIFT_SPEED)
                        setYSpeed(-SHIFT_SPEED);
                } else {
                    if (getXSpeed() > -SHIFT_SPEED)
                        setXSpeed(-SHIFT_SPEED);
                }
                break;
        }
        inBypass = true;
    }

    return inBypass;
}


///////////////////////////////////////////////////////////
// Вращение танка

void Tank::rotation(float dt) {
    // При смене направления движения на 90 градусов производится обновление габаритов
    // танка вместе с его координатной точкой. Также запускается процесс вращения спрайта
    if (_oldDirection != getDirection()) {
        _rotation = true;

        switch (getDirection()) {
            // Если направления отличаются на 180 градусов,
            // то производить перерасчёт габаритов не нужно
            case Direction::UP :
                if (_oldDirection == Direction::DOWN) {
                    // Предыдущее направление сразу приравнивается к новому
                    _oldDirection = getDirection();
                    break;
                }
            case Direction::DOWN : {
                if (_oldDirection == Direction::UP) {
                    _oldDirection = getDirection();
                    break;
                }

                float newY = getY() - _offset;
                float newX = getX() + _offset;
                float newWidth = level::tank::WIDTH;
                float newHeight = level::tank::HEIGHT;

                // Необработка части габаритов танка, чтобы он мог развернуться
                // в пределах квадрата 2х2 на Т и Г образных перекрёстках
                float uncollisionPart = level::tank::WIDTH / 10.0f;

                bool topIntersects = bool(
                    getGame().checkIntersects(newX + uncollisionPart, newY,
                    newWidth - uncollisionPart * 2.0f, _offset, this));
                bool bottomIntersects = bool(
                    getGame().checkIntersects(newX + uncollisionPart, getY() + getHeight(),
                    newWidth - uncollisionPart * 2.0f, _offset, this));

                // Если танк зажат сверху и снизу,
                // и его новые габариты не помещаются на текущем месте,
                // то отменить смену направления
                if (topIntersects && bottomIntersects) {
                    setDirection(_oldDirection);
                } else {
                    setY(newY);
                    setX(newX);
                    setWidth(newWidth);
                    setHeight(newHeight);
                    _oldDirection = getDirection();
                }
                break;
            }

            case Direction::RIGHT :
                if (_oldDirection == Direction::LEFT) {
                    _oldDirection = getDirection();
                    break;
                }
            case Direction::LEFT : {
                if (_oldDirection == Direction::RIGHT) {
                    _oldDirection = getDirection();
                    break;
                }

                float newY = getY() + _offset;
                float newX = getX() - _offset;
                float newWidth = level::tank::HEIGHT;
                float newHeight = level::tank::WIDTH;

                float uncollisionPart = level::tank::WIDTH / 10.0f;

                bool rightIntersects = bool(
                    getGame().checkIntersects(getX() + getWidth(), newY + uncollisionPart,
                    _offset, newHeight - uncollisionPart * 2.0f, this));
                bool leftIntersects = bool(
                    getGame().checkIntersects(newX, newY + uncollisionPart, _offset,
                    newHeight - uncollisionPart * 2.0f, this));

                if (rightIntersects && leftIntersects) {
                    setDirection(_oldDirection);
                } else {
                    setY(newY);
                    setX(newX);
                    setWidth(newWidth);
                    setHeight(newHeight);
                    _oldDirection = getDirection();
                }
                break;
            }

            default:
                break;
        }
    }
}

bool Tank::spriteRotation(float dt) {
    // RIGHT = 90, DOWN = 180, LEFT = 270, UP = 360
    int targetRotation = 90 * (int)getDirection();

    float anglePerSecond = 90.0f / _rotationTime;

    // По знаку разницы углов делается вывод, в каком направлении производить поворот
    float deltaRotation = float(targetRotation) - _rotationAngle;
    // Если разница между текущим и целевым углом больше 180 градусов,
    // то, явно происходит смена направления с UP(360) на RIGHT(90) или наоборот
    if (std::abs(deltaRotation) > 180.0f) {
        if (_rotationAngle >= 270.0f)  // Если текущий угол уже на 270ти или выше,
            _rotationAngle -= 360.0f;  // то отмотать оборот против часовой для RIGHT(90)
        else if (_rotationAngle >= -90.0f)  // Если текущий угол где-то в начале, то
            _rotationAngle += 360.0f;       // добавить оборот для UP(360)

        deltaRotation = -deltaRotation; //Смена направления вращения
    }
    if (deltaRotation >= 0.0f)
        _rotationClockwise = true;
    else
        _rotationClockwise = false;

    _rotationAngle += _rotationClockwise ? +(anglePerSecond * dt)
                                         : -(anglePerSecond * dt);
    _spriteEntity->setRotation(_rotationAngle);

    // Точная постановка на заданный угол при его достижении или "выбега" за него
    if (_rotationClockwise ? _rotationAngle >= targetRotation
                           : _rotationAngle <= targetRotation) {
        _rotationAngle = (float)targetRotation;
        _spriteEntity->setRotation(_rotationAngle);
        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////
// Блок методов, реализующих выстрел танка

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

    // Установка скорости полёта снаряда и его центровка относительно
    // своих размеров к дулу орудия танка
    switch (getDirection()) {
        case Direction::LEFT :
            bulletSpeedX = -level::bullet::basic::SPEED;
            bulletPositionY -= (level::bullet::basic::WIDTH / 2.0f);
            bulletPositionX -= level::bullet::basic::HEIGHT;
            break;

        case Direction::RIGHT :
            bulletSpeedX = level::bullet::basic::SPEED;
            bulletPositionY -= (level::bullet::basic::WIDTH / 2.0f);
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
        bullet.setTextureRect(level::bullet::basic::IMAGE); // Для поворота спрайта
    }

    //Отдача от выстрела
    _currentSpeed -= getMaxSpeed() / 2.0f;
}


void Tank::calculateFrontCellPosition(float& x, float& y) {
    // Дополнительная длина до дула танка
    float distanceToGunpoint = 0.0f;

    switch (getType()) {
        case GameObjectType::TANK_FIRST_PLAYER :
        case GameObjectType::TANK_SECOND_PLAYER :
        case GameObjectType::TANK_ENEMY :
            distanceToGunpoint = 0.4f;
            break;
        default :
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

        default :
            break;
    }
}