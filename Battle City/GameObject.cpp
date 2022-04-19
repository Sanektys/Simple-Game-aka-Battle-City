#include "GameObject.h"
#include "Game.h"
#include "CoordPoint.h"
#include "Utils.h"


// Скорость выталкивания объекта из другого объекта
const float SHIFT_SPEED{0.5f};


GameObject::GameObject(const class Game& game) : _game(game) {
    _group = GameObjectGroup::NONE;
    _type = GameObjectType::NONE;
    _direction = Direction::UP;
}

void GameObject::render(sf::RenderWindow* rw) {
    if (_spriteTerrain) {
        // Помещение спрайта на игровом поле.
        // Предполагается, что центр спрайта смещён на центр самой текстуры
        _spriteTerrain->setPosition(getX() * level::PIXELS_PER_CELL
                                    + (getWidth() * level::PIXELS_PER_CELL) / 2.0f,
                                    getY() * level::PIXELS_PER_CELL
                                    + (getHeight() * level::PIXELS_PER_CELL) / 2.0f);
        rw->draw(*_spriteTerrain);
    }
    else if (_spriteEntity) {
        _spriteEntity->setPosition(getX() * level::PIXELS_PER_CELL
                                   + (getWidth() * level::PIXELS_PER_CELL) / 2.0f,
                                   getY() * level::PIXELS_PER_CELL
                                   + (getHeight() * level::PIXELS_PER_CELL) / 2.0f);
        rw->draw(*_spriteEntity);
    }
}

void GameObject::update(float dt) {
    switch (getType()) {
        case GameObjectType::TANK_ENEMY :
        case GameObjectType::TANK_FIRST_PLAYER :
        case GameObjectType::TANK_SECOND_PLAYER : {
            // Если в данный момент объект не проходит намеренно сквозь другой объект
            if (!getInBypass()) {
                // То произвести проверку на коллизию
                _isSticking = bool(
                    getGame().checkIntersects(getX(), getY(),
                                              getWidth(), getHeight(), this));
            }
            // Если объект застрял в другом объекте
            if (getIsSticking())
                escapeSticking();
        }

        case GameObjectType::BULLET :
        case GameObjectType::BULLET_TRACER : {
            float oldX = getX();
            float oldY = getY();

            float newX = getX() + getXSpeed() * dt; // Новая позиция по горизонтали
            float newY = getY() + getYSpeed() * dt; // Новая позиция по вертикали
            // Новые координаты высчитываются по текущей скорости и дельте времени,
            // затраченной на предыдущий такт обновления игровой логики

            // Если одна из двух координат не совпадает с предыдущей,
            // то объект перемещается
            if ((oldX != newX || oldY != newY) && !_isSticking && !_inBypass) {
                getGame().moveObjectTo(this, newX, newY);
            } else {
                setX(newX);
                setY(newY);
            }
        }
        break;

        default :
            break;
    }
}

void GameObject::escapeSticking() {
    // Условное разделение текущего объекта на четыре равные части
    CoordPoint topLeftPoint    (this, TypeCoordPoint::TOP_LEFT);
    CoordPoint topRightPoint   (this, TypeCoordPoint::TOP_RIGHT);
    CoordPoint bottomRightPoint(this, TypeCoordPoint::BOTTOM_RIGHT);
    CoordPoint bottomLeftPoint (this, TypeCoordPoint::BOTTOM_LEFT);

    // Проверка на коллизию с другим объектом каждой четверти текущего объекта
    /////////////////////////
    bool topLeftIntersect = bool(getGame().checkIntersects(
        topLeftPoint.coordX, topLeftPoint.coordY,
        topLeftPoint.width, topLeftPoint.height, this));

    bool topRightIntersect = bool(getGame().checkIntersects(
        topRightPoint.coordX, topRightPoint.coordY,
        topRightPoint.width, topRightPoint.height, this));

    bool bottomRightIntersect = bool(getGame().checkIntersects(
        bottomRightPoint.coordX, bottomRightPoint.coordY,
        bottomRightPoint.width, bottomRightPoint.height, this));

    bool bottomLeftIntersect = bool(getGame().checkIntersects(
        bottomLeftPoint.coordX, bottomLeftPoint.coordY,
        bottomLeftPoint.width, bottomLeftPoint.height, this));

    // Две соседние четверти одновременно имеют коллизию
    bool primarySet{false};

    // Первичные проверки на то, что две соседние части объекта
    // находятся внутри других объектов
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

    // Если только одна четверть имеет коллизию
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
            default :
                break;
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
            default :
                break;
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
            default :
                break;
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
            default :
                break;
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