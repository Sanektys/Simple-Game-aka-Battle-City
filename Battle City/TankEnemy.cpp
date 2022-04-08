#include <random>

#include "TankEnemy.h"
#include "CoordPoint.h"
#include "Utils.h"
#include "Wall.h"
#include "Bullet.h"
#include "Game.h"


extern std::mt19937* RANDOM;


TankEnemy::TankEnemy(const class Game& game, sf::IntRect rect) : Tank(game) {
    setType(GameObjectType::TANK_ENEMY);

    setHealth(level::tank::enemy::basic::HEALTH);
    setMaxSpeed(level::tank::enemy::basic::SPEED);
    setSpeedup(10.0f);
    setBrakingSpeed(8.0f);
    setRotationTime(0.9f);

    _spriteEntity->setTextureRect(rect);
}

TankEnemy::~TankEnemy() {
    getGame().increaseDiedEnemiesCount();
}

void TankEnemy::update(float dt) {
    ai(dt);

    Tank::update(dt);
}

void TankEnemy::ai(float dt) {
    if (isAvoidSolidBarrier(dt))
        return;

    bool isChangeDirection{false};
    if (_changeDirectionPauseTime <= 0.0f) {
        _changeDirectionPauseTime = 1.0f;
        isChangeDirection = (*RANDOM)() % 5 == 0;
    } else {
        _changeDirectionPauseTime -= dt;
    }
    if (isChangeDirection) {
        // Двигаться в случайном направлении
        changeDirection(dt);
        return;
    }

    float x = 0.0f;
    float y = 0.0f;
    calculateFrontCellPosition(x, y);
        
    float xDelta = 0.0f;
    float yDelta = 0.0f;
        
    switch (getDirection()) {
        case Direction::LEFT :
            xDelta = -1.0f;
            break;
            
        case Direction::RIGHT :
            xDelta = +1.0f;
            break;
            
        case Direction::UP :
            yDelta = -1.0f;
            break;
            
        case Direction::DOWN :
            yDelta = +1.0f;
            break;
            
        default:
            break;
    }

    // Поиск ближайших объектов с дистанцией до них
    std::unique_ptr<GameObject>* object{nullptr};
    int fireDistance{0};
    do {
        object = &getGame().checkIntersects(x - 0.05f, y - 0.05f,
                                            0.1f, 0.1f, this);
        x += xDelta;
        y += yDelta;
        fireDistance += 1;
    } while (!*object
             && fireDistance <= level::tank::enemy::basic::MAX_FIRE_DISTANCE);

    if (*object)
        switch ((*object)->getType()) {
            case GameObjectType::BULLET :
                if (dynamic_cast<Bullet&>(**object).getOwnerType()
                    == GameObjectType::TANK_FIRST_PLAYER
                        ||
                    dynamic_cast<Bullet&>(**object).getOwnerType()
                    == GameObjectType::TANK_SECOND_PLAYER) {
                    Tank::move(Direction::NONE, dt);
                    fire();
                    return;
                }
                break;

            case GameObjectType::WALL :
                if (dynamic_cast<Wall&>(**object).getInvulnerable() == false) {
                    Tank::move(Direction::NONE, dt);
                    fire();
                    return;
                }
                break;

            case GameObjectType::SOLID_WALL :
            case GameObjectType::BASE :
            case GameObjectType::TANK_FIRST_PLAYER :
            case GameObjectType::TANK_SECOND_PLAYER :
                Tank::move(Direction::NONE, dt);
                fire();
                return;
                break;
                
            default:
                break;
        }

    // 10% вероятность изменения направления
    if (_changeDirectionPauseTime <= 0.0f) {
        _changeDirectionPauseTime = 1.0f;
        isChangeDirection = (*RANDOM)() % 10 == 0;
    } else {
        _changeDirectionPauseTime -= dt;
    }
    if (isChangeDirection) {
        // Двигаться в случайном направлении
        changeDirection(dt);
        return;
    }

    Tank::move(getDirection(), dt);
}

bool TankEnemy::isAvoidSolidBarrier(float dt) {
    switch (getDirection()) {
        case Direction::UP : {
            CoordPoint topLeft(this, TypeCoordPoint::TOP_LEFT);
            CoordPoint topRight(this, TypeCoordPoint::TOP_RIGHT);

            bool leftBarrier = bool(getGame().checkIntersects(
                topLeft.coordX, topLeft.coordY - 0.05f,
                topLeft.width, topLeft.height, this));
            bool rightBarrier = bool(getGame().checkIntersects(
                topRight.coordX, topRight.coordY - 0.05f,
                topRight.width, topRight.height, this));

            if ((leftBarrier || rightBarrier) && !getInBypass()) {
                changeDirection(dt);
                return true;
            }
        }
        break;

        case Direction::RIGHT : {
            CoordPoint topRight(this, TypeCoordPoint::TOP_RIGHT);
            CoordPoint bottomRight(this, TypeCoordPoint::BOTTOM_RIGHT);

            bool topBarrier = bool(getGame().checkIntersects(
                topRight.coordX + 0.05f, topRight.coordY,
                topRight.width, topRight.height, this));
            bool bottomBarrier = bool(getGame().checkIntersects(
                bottomRight.coordX + 0.05f, bottomRight.coordY,
                bottomRight.width, bottomRight.height, this));

            if ((topBarrier || bottomBarrier) && !getInBypass()) {
                changeDirection(dt);
                return true;
            }
        }
        break;

        case Direction::DOWN : {
            CoordPoint bottomLeft(this, TypeCoordPoint::BOTTOM_LEFT);
            CoordPoint bottomRight(this, TypeCoordPoint::BOTTOM_RIGHT);

            bool leftBarrier = bool(getGame().checkIntersects(
                bottomLeft.coordX, bottomLeft.coordY + 0.05f,
                bottomLeft.width, bottomLeft.height, this));
            bool rightBarrier = bool(getGame().checkIntersects(
                bottomRight.coordX, bottomRight.coordY + 0.05f,
                bottomRight.width, bottomRight.height, this));

            if ((leftBarrier || rightBarrier) && !getInBypass()) {
                changeDirection(dt);
                return true;
            }
        }
        break;

        case Direction::LEFT : {
            CoordPoint topLeft(this, TypeCoordPoint::TOP_LEFT);
            CoordPoint bottomLeft(this, TypeCoordPoint::BOTTOM_LEFT);

            bool topBarrier = bool(getGame().checkIntersects(
                topLeft.coordX - 0.05f, topLeft.coordY,
                topLeft.width, topLeft.height, this));
            bool bottomBarrier = bool(getGame().checkIntersects(
                bottomLeft.coordX - 0.05f, bottomLeft.coordY,
                bottomLeft.width, bottomLeft.height, this));

            if ((topBarrier || bottomBarrier) && !getInBypass()) {
                changeDirection(dt);
                return true;
            }
        }
        break;

        default:
            break;
    }
    return false;
}

void TankEnemy::changeDirection(float dt) {
    Direction prevDirection = getDirection();
    Direction newDirection{Direction::NONE};
    do
        newDirection = (Direction)((*RANDOM)() % ((unsigned)Direction::MAX - 1u) + 1u);
    while (prevDirection == newDirection);
    
    Tank::move(newDirection, dt);
}