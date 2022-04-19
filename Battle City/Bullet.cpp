#include "Bullet.h"
#include "BulletTracer.h"
#include "GameObjectType.h"
#include "Level.h"
#include "Game.h"


Bullet::Bullet(const class Game& game, sf::IntRect rect,
               enum Direction direction, float speedX, float speedY,
               enum GameObjectType owner)
    : GameObject(game), _ownerType(owner) {
    setGroup(GameObjectGroup::ENTITY);
    setType(GameObjectType::BULLET);

    setDirection(direction);
    setXSpeed(speedX);
    setYSpeed(speedY);

    setWidth(level::bullet::basic::WIDTH);
    setHeight(level::bullet::basic::HEIGHT);

    _spriteEntity.reset(new sf::Sprite());
    _spriteEntity->setTexture(*level::ATLAS_ENTITY);
    _spriteEntity->setOrigin(level::bullet::basic::PIXELS_WIDTH / 2.0f,
                             level::bullet::basic::PIXELS_HEIGHT / 2.0f);
    setTextureRect(rect);
}

Bullet::~Bullet() {
    // Сообщение всем имеющимся трассерам, что снаряд уничтожен
    if (_firstTracer)  _firstTracer->bulletDestroyed();
    if (_secondTracer) _secondTracer->bulletDestroyed();
    if (_thirdTracer)  _thirdTracer->bulletDestroyed();
}

void Bullet::update(float dt) {
    bool createNewTrace{false};
    switch (getDirection()) {
        case Direction::UP :
        case Direction::DOWN :
            _spanDistance += std::abs(getYSpeed()) * dt;          
            break;
            
        case Direction::RIGHT :
        case Direction::LEFT :
            _spanDistance += std::abs(getXSpeed()) * dt;
            break;

        default :
            break;
    }
    // Создание трёх трассеров по мере пролёта снаряда
    if (!_firstTracer && _spanDistance >= BulletTracer::BASIC_HEIGHT)
        createNewTrace = true;
    else if (!_secondTracer && _spanDistance >= BulletTracer::BASIC_HEIGHT * 2.0F)
        createNewTrace = true;
    else if (!_thirdTracer && _spanDistance >= BulletTracer::BASIC_HEIGHT * 3.0F)
        createNewTrace = true;

    if (createNewTrace && (!_firstTracer || !_secondTracer || !_thirdTracer)) {
        float spawnX{0.0f};
        float spawnY{0.0f};

        switch (getDirection()) {
            case Direction::UP :
                if      (!_firstTracer)  spawnY = getY() + getHeight();
                else if (!_secondTracer) spawnY = getY() + getHeight()
                                                  + BulletTracer::BASIC_HEIGHT;
                else if (!_thirdTracer)  spawnY = getY() + getHeight()
                                                  + BulletTracer::BASIC_HEIGHT * 2.0f;

                spawnX = getX() + getWidth() / 2.0f - BulletTracer::BASIC_WIDTH / 2.0f;
                break;

            case Direction::DOWN :
                if      (!_firstTracer)  spawnY = getY() - BulletTracer::BASIC_HEIGHT;
                else if (!_secondTracer) spawnY = getY() - BulletTracer::BASIC_HEIGHT * 2.0f;
                else if (!_thirdTracer)  spawnY = getY() - BulletTracer::BASIC_HEIGHT * 3.0f;

                spawnX = getX() + getWidth() / 2.0f - BulletTracer::BASIC_WIDTH / 2.0f;
                break;

            case Direction::RIGHT :
                if      (!_firstTracer)  spawnX = getX() - BulletTracer::BASIC_HEIGHT;
                else if (!_secondTracer) spawnX = getX() - BulletTracer::BASIC_HEIGHT * 2.0f;
                else if (!_thirdTracer)  spawnX = getX() - BulletTracer::BASIC_HEIGHT * 3.0f;

                spawnY = getY() + getHeight() / 2.0f - BulletTracer::BASIC_WIDTH / 2.0f;
                break;

            case Direction::LEFT :
                if      (!_firstTracer)  spawnX = getX() + getWidth();
                else if (!_secondTracer) spawnX = getX() + getWidth()
                                                  + BulletTracer::BASIC_HEIGHT;
                else if (!_thirdTracer)  spawnX = getX() + getWidth()
                                                  + BulletTracer::BASIC_HEIGHT * 2.0f;

                spawnY = getY() + getHeight() / 2.0f - BulletTracer::BASIC_WIDTH / 2.0f;
                break;

            default :
                break;
        }

        BulletTracer* tracer = dynamic_cast<BulletTracer*>(&*getGame().
            createObject(GameObjectType::BULLET_TRACER, spawnX, spawnY));

        if (tracer) {
            tracer->setOwner(this);
            tracer->setXSpeed(this->getXSpeed());
            tracer->setYSpeed(this->getYSpeed());
            tracer->setDirection(this->getDirection());

            if (!_firstTracer) {
                tracer->setGradation(3);
                _firstTracer = tracer;
            }
            else if (!_secondTracer) {
                tracer->setGradation(2);
                _secondTracer = tracer;
            }
            else if (!_thirdTracer) {
                tracer->setGradation(1);
                _thirdTracer = tracer;
            }
            tracer->setShapeRect();  // Финальная установка параметров трассера
        }
    }

    GameObject::update(dt);
}

void Bullet::intersect(class GameObject* object) {
    // Самоуничтожение
    setHealth(0);

    // Нанести урон по цели
    object->doDamage(1);
}

void Bullet::setTextureRect(sf::IntRect rect) {
    // Установка спрайта на уникальную, для каждого типа танка,
    // текстуру из атласа
    // Кейс на танк первого игрока отсутствует,
    // т.к. он установлен по умолчанию
    switch (getOwnerType()) {
        case GameObjectType::TANK_SECOND_PLAYER :
            rect.left = level::bullet::basic::PIXELS_WIDTH;
            break;  // Смещение спрайта по атласу налево

        case GameObjectType::TANK_ENEMY :
            rect.left = level::bullet::basic::PIXELS_WIDTH * 2;
            break;

        default :
            break;
    }
    _spriteEntity->setTextureRect(rect);
    // Поворот спрайта на основе направления движения
    // Функция поворачивает по часовой стрелке(а не как в тригонометрии)
    _spriteEntity->setRotation(90.0f * (float)getDirection());
}

void Bullet::setDirection(enum Direction direction) {
    // Инверсия дефолтных размеров снаряда если он летит по горизонтальной оси
    switch (direction) {
        case Direction::LEFT :
        case Direction::RIGHT : {
            float temp{getWidth()};
            setWidth(getHeight());
            setHeight(temp);
            break;
        }

        default :
            break;
    }
    GameObject::setDirection(direction);
}