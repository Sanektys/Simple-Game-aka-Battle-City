#include "Bullet.h"
#include "GameObjectType.h"
#include "Level.h"


Bullet::Bullet() {
	_type = GameObjectType::BULLET;
	_ownerType = GameObjectType::NONE;

	setWidth(STANDART_BULLET_WIDTH);
	setHeight(STANDART_BULLET_HEIGHT);

	_spriteEntity = new sf::Sprite();
	_spriteEntity->setTexture(*_atlasEntity);
	_spriteEntity->setOrigin((getWidth() * (float)PIXELS_PER_CELL) / 2.0f,
		                     (getHeight() * (float)PIXELS_PER_CELL) / 2.0f);
}

Bullet::~Bullet() {
	if (_spriteEntity) {
		delete _spriteEntity;
		_spriteEntity = nullptr;
	}
}

void Bullet::render(sf::RenderWindow* rw) {	
	GameObject::render(rw);
}

void Bullet::setTextureRect(sf::IntRect rect) {	
	switch (_ownerType) {
	    case GameObjectType::TANK_SECOND_PLAYER : {
			rect.left = 6;
			break;
	    }
		case GameObjectType::TANK_ENEMY : {
			rect.left = 12;
			break;
		}
	}

	switch (getDirection()) {
	    case Direction::LEFT :
	    case Direction::RIGHT : {
			setWidth(STANDART_BULLET_HEIGHT);
			setHeight(STANDART_BULLET_WIDTH);
		    break;
	    }
	    default : {
			setWidth(STANDART_BULLET_WIDTH);
			setHeight(STANDART_BULLET_HEIGHT);
		    break;
	    }
	}

	_spriteEntity->setRotation(90.0f * (int)getDirection());
	_spriteEntity->setTextureRect(rect);
}

void Bullet::intersect(class GameObject* object) {
	// ���������������
	setHealth(0);

	// ������� ���� �� ����
	object->doDamage(1);
}