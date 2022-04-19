#include "BulletTracer.h"
#include "Bullet.h"


const float BulletTracer::BASIC_WIDTH {0.1F};
const float BulletTracer::BASIC_HEIGHT{1.5F};

BulletTracer::BulletTracer(const class Game& game) 
    : GameObject(game) {
    setGroup(GameObjectGroup::ENTITY);
    setType(GameObjectType::BULLET_TRACER);

    setPhysical(false);
}

void BulletTracer::update(float dt) {
    if (_owner) {  // ≈сли снар€д-владелец существует, то получить его координаты
        _finalCoordX = _owner->getX();
        _finalCoordY = _owner->getY();
    }
    // ≈сли трассер "перегнал" снар€д, то он уничтожаетс€
    // “.е. если снар€д уничтожен, то как только трассер пересекает его последние
    // координаты, он также уничтожаетс€
    switch (getDirection()) {
        case Direction::UP :
            if (getY() <= _finalCoordY)
                setHealth(0);
            break;

        case Direction::DOWN :
            if (getY() + getHeight() >= _finalCoordY)
                setHealth(0);
            break;

        case Direction::RIGHT :
            if (getX() + getWidth() >= _finalCoordX)
                setHealth(0);
            break;

        case Direction::LEFT :
            if (getX() <= _finalCoordX)
                setHealth(0);
            break;

        default :
            break;
    }

    GameObject::update(dt);
}

void BulletTracer::render(sf::RenderWindow* rw) {
    _tracerShape.setPosition(getX() * level::PIXELS_PER_CELL
                             + getWidth() * level::PIXELS_PER_CELL / 2.0f,
                             getY() * level::PIXELS_PER_CELL
                             + getHeight() * level::PIXELS_PER_CELL / 2.0f);
    rw->draw(_tracerShape);
}

void BulletTracer::setDirection(enum Direction direction) {
    // ”становка первичного значени€
    // конечной координаты чуть дальше переда трассера
    switch (direction) {
        case Direction::UP :
            _finalCoordY = getY() - 0.000001f;
            break;

        case Direction::DOWN :
            _finalCoordY = getY() + BASIC_HEIGHT + 0.000001f;
            break;

        case Direction::RIGHT :
            _finalCoordX = getX() + BASIC_HEIGHT + 0.000001f;
            break;

        case Direction::LEFT :
            _finalCoordX = getX() - 0.000001f;
            break;

        default :
            break;
    }
    GameObject::setDirection(direction);
}

void BulletTracer::setShapeRect() {
    sf::Color color;
    switch (dynamic_cast<const Bullet*>(_owner)->getOwnerType()) {
        // ”становка габаритов и цвета трассера дл€ каждого типа техники
        case GameObjectType::TANK_ENEMY :
            setWidth(BASIC_WIDTH);
            setHeight(BASIC_HEIGHT);
            color = sf::Color(128, 115, 115);
            switch (_gradation) {  // ¬ыбор прозрачности
                case 3 :
                    color.a = 30;
                    break;
                case 2 :
                    color.a = 20;
                    break;
                case 1 :
                    color.a = 10;
                    break;
            }
            _tracerShape.setFillColor(color);
            break;

        case GameObjectType::TANK_FIRST_PLAYER :
            setWidth(BASIC_WIDTH);
            setHeight(BASIC_HEIGHT);
            color = sf::Color(120, 128, 115);
            switch (_gradation) {
                case 3 :
                    color.a = 30;
                    break;
                case 2 :
                    color.a = 20;
                    break;
                case 1 :
                    color.a = 10;
                    break;
            }
            _tracerShape.setFillColor(color);
            break;

        case GameObjectType::TANK_SECOND_PLAYER :
            setWidth(BASIC_WIDTH);
            setHeight(BASIC_HEIGHT);
            color = sf::Color(128, 125, 115);
            switch (_gradation) {
                case 3 :
                    color.a = 30;
                    break;
                case 2 :
                    color.a = 20;
                    break;
                case 1 :
                    color.a = 10;
                    break;
            }
            _tracerShape.setFillColor(color);
            break;

        default :
            break;
    }
    // »зменение габаритов согласно ориентации снар€да
    switch (_owner->getDirection()) {
        case Direction::UP :
        case Direction::DOWN : {
            setWidth(getWidth());
            setHeight(getHeight());           
            break;
        }

        case Direction::RIGHT :
        case Direction::LEFT : {
            float temp{getWidth()};
            setWidth(getHeight());
            setHeight(temp);
            break;
        }

        default :
            break;
    }
    _tracerShape.setSize(sf::Vector2f(getWidth()  * level::PIXELS_PER_CELL,
                                      getHeight() * level::PIXELS_PER_CELL));
    _tracerShape.setOrigin(getWidth()  * level::PIXELS_PER_CELL / 2.0f,
                           getHeight() * level::PIXELS_PER_CELL / 2.0f);
}