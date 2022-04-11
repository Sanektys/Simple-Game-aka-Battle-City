#include "Bullet.h"
#include "GameObjectType.h"
#include "Level.h"


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

void Bullet::intersect(class GameObject* object) {
    // ���������������
    setHealth(0);

    // ������� ���� �� ����
    object->doDamage(1);
}

void Bullet::setTextureRect(sf::IntRect rect) {
    // ��������� ������� �� ����������, ��� ������� ���� �����,
    // �������� �� ������
    // ���� �� ���� ������� ������ �����������,
    // �.�. �� ���������� �� ���������
    switch (getOwnerType()) {
        case GameObjectType::TANK_SECOND_PLAYER :
            rect.left = level::bullet::basic::PIXELS_WIDTH;
            break;  // �������� ������� �� ������ ������

        case GameObjectType::TANK_ENEMY :
            rect.left = level::bullet::basic::PIXELS_WIDTH * 2;
            break;

        default :
            break;
    }
    // ������� ������� �� ������ ����������� ��������
    // ������� ������������ �� ������� �������(� �� ��� � �������������)
    _spriteEntity->setRotation(90.0f * (float)getDirection());
    _spriteEntity->setTextureRect(rect);
}

void Bullet::setDirection(enum Direction direction) {
    // �������� ��������� �������� ������� ���� �� ����� �� �������������� ���
    switch (getDirection()) {
        case Direction::LEFT :
        case Direction::RIGHT : {
            float offset = (getHeight() - getWidth()) / 2.0f;
            setX(getX() - offset);  // �������� ��������� ���������
            setY(getY() + offset);  // ��-�� "��������" ���������
            setWidth(level::bullet::basic::HEIGHT);
            setHeight(level::bullet::basic::WIDTH);
            break;
        }

        default :
            break;
    }
    GameObject::setDirection(direction);
}