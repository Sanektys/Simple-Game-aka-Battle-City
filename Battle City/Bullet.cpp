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

	setWidth(STANDART_BULLET_WIDTH);
	setHeight(STANDART_BULLET_HEIGHT);

	_spriteEntity.reset(new sf::Sprite());
	_spriteEntity->setTexture(*ATLAS_ENTITY);
	_spriteEntity->setOrigin((getWidth() * (float)PIXELS_PER_CELL) / 2.0f,
		                     (getHeight() * (float)PIXELS_PER_CELL) / 2.0f);
    setTextureRect(rect);
}

void Bullet::render(sf::RenderWindow* rw) {	
	GameObject::render(rw);
}

void Bullet::setTextureRect(sf::IntRect rect) {	
	switch (_ownerType) {
	    case GameObjectType::TANK_SECOND_PLAYER :
			rect.left = 6;
			break;
 
		case GameObjectType::TANK_ENEMY :
			rect.left = 12;
			break;
	}
	switch (getDirection()) {
	    case Direction::LEFT :
	    case Direction::RIGHT :
			setWidth(STANDART_BULLET_HEIGHT);
			setHeight(STANDART_BULLET_WIDTH);
		    break;

        default :
			setWidth(STANDART_BULLET_WIDTH);
			setHeight(STANDART_BULLET_HEIGHT);
		    break;
	}
	_spriteEntity->setRotation(90.0f * (int)getDirection());
	_spriteEntity->setTextureRect(rect);
}

void Bullet::intersect(class GameObject* object) {
	// Самоуничтожение
	setHealth(0);

	// Нанести урон по цели
	object->doDamage(1);
}