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

    setSpeedup(level::tank::enemy::basic::SPEED);
    setBrakingSpeed(level::tank::enemy::basic::SPEED);
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
    // Сначала проверяется столкновение с преградой
    if (isAvoidSolidBarrier(dt))
        return;

    // Для того, чтобы танк не расстреливал бесконечно одну линию блоков,
    // установлен шанс смены направления до входа в секцию стрельбы  
    bool isChangeDirection{false};
    if (_changeDirectionPauseTime <= 0.0f && getDirection() == Direction::NONE) {
        _changeDirectionPauseTime = 1.0f;
        // 12.5% - вероятность изменения направления
        isChangeDirection = (*RANDOM)() % 8 == 0;
    } else {
        _changeDirectionPauseTime -= dt;
    }
    if (isChangeDirection) {
        changeDirection(dt);  // Сменить направление движения
        return;
    }

    // Блок поиска возможных целей по направлению орудия

    float x = 0.0f;
    float y = 0.0f;
    calculateFrontCellPosition(x, y);
       
    float xDelta = 0.0f;  // Шаг смещения по горизонтали при поиске   
    float yDelta = 0.0f;  // Шаг смещения по вертикали при поиске
        
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

    std::unique_ptr<GameObject>* object{nullptr};
    float fireDistanceX{0.0f};  // Текущая дистанция стрельбы по горизонтали
    float fireDistanceY{0.0f};  // Текущая дистанция стрельбы по вертикали
    bool isNotMaxFireX{true};
    bool isNotMaxFireY{true};
    do {
        object = &getGame().checkIntersects(
            x - level::bullet::basic::WIDTH / 2.0f,
            y - level::bullet::basic::WIDTH / 2.0f,
            level::bullet::basic::WIDTH, level::bullet::basic::WIDTH, this);
        x += xDelta;
        y += yDelta; 
        fireDistanceX += std::abs(xDelta);
        fireDistanceY += std::abs(yDelta);
        isNotMaxFireX = fireDistanceX <= level::tank::enemy::basic::MAX_FIRE_DISTANCE_X;
        isNotMaxFireY = fireDistanceY <= level::tank::enemy::basic::MAX_FIRE_DISTANCE_Y;
    } while (!*object && (isNotMaxFireX && isNotMaxFireY));

    // Если объект найден
    if (*object)
        switch ((*object)->getType()) {
            case GameObjectType::BULLET : // Только снаряды противников
                if (dynamic_cast<Bullet&>(**object).getOwnerType()
                    == GameObjectType::TANK_FIRST_PLAYER
                        ||
                    dynamic_cast<Bullet&>(**object).getOwnerType()
                    == GameObjectType::TANK_SECOND_PLAYER) {
                    // Остановка танка
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
    // Если объект не найден, или он не является целью для атаки,
    // то продолжить движение

    // Если не активировался первый блок смены направления при остановке,
    // то обрабатывается этот
    if (_changeDirectionPauseTime <= 0.0f) {
        _changeDirectionPauseTime = 1.0f;
        // 5% вероятность изменения направления
        isChangeDirection = (*RANDOM)() % 20 == 0;
    } else {
        _changeDirectionPauseTime -= dt;
    }
    if (isChangeDirection) {
        changeDirection(dt);
        return;
    }

    // Если ни одно из действий выше не обработано, то продолжить движение
    Tank::move(getDirection(), dt);
}

bool TankEnemy::isAvoidSolidBarrier(float dt) {
    switch (getDirection()) {
        case Direction::UP : {
            // Для проверки столкновения берутся две половинки переда танка
            CoordPoint topLeft(this, TypeCoordPoint::TOP_LEFT);
            CoordPoint topRight(this, TypeCoordPoint::TOP_RIGHT);

            // Каждая половинка проверяется на пересечение с преградой
            // перед танком, для этого их координаты слегка смещаются 
            // на 0.05 пунков вперёд коллизии лба танка
            bool leftBarrier = bool(getGame().checkIntersects(
                topLeft.coordX, topLeft.coordY - 0.05f,
                topLeft.width, topLeft.height, this));
            bool rightBarrier = bool(getGame().checkIntersects(
                topRight.coordX, topRight.coordY - 0.05f,
                topRight.width, topRight.height, this));

            // Если одна половина имеет пересечение с преградой
            // и танк не огибает эту помеху
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
    Direction currentDirection = getDirection();
    Direction newDirection{Direction::NONE};
    do  // Выбор случайного направления от 1(RIGHT) до 4(UP)
        newDirection = (Direction)((*RANDOM)() % ((unsigned)Direction::MAX - 1u) + 1u);
        // Выбор осуществляется до тех пор, пока новое направление равно текущему
        // и предыдущему. Однако, если танк зажат сверху и снизу или слева и справа,
        // то он всё равно сможет взять предыдущее направление
    while (!(newDirection != currentDirection && newDirection != _prevDirection));
    _prevDirection = currentDirection;
    
    Tank::move(newDirection, dt);
}