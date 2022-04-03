#include "Bullet.h"
#include "GameObjectType.h"
#include "Level.h"


Bullet::Bullet(const class Game& game, sf::IntRect rect,
               enum class Direction direction, float speedX, float speedY,
               enum class GameObjectType owner)
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
	}    
    // Поворот спрайта на основе направления движения
    // Функция поворачивает по часовой стрелке(а не как в тригонометрии)
	_spriteEntity->setRotation(90.0f * (int)getDirection());
	_spriteEntity->setTextureRect(rect);
}

void Bullet::setDirection(enum class Direction direction) {
    // Инверсия дефолтных размеров снаряда если он летит по горизонтальной оси
    switch (getDirection()) {
        case Direction::LEFT:
        case Direction::RIGHT:
            setWidth(level::bullet::basic::HEIGHT);
            setHeight(level::bullet::basic::WIDTH);
            break;
    }
    GameObject::setDirection(direction);
}