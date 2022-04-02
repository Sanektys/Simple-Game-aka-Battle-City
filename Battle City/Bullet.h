#pragma once

#include "GameObject.h"
#include "Tank.h"


class Bullet : public GameObject
{
    public :
        Bullet(const class Game& game, sf::IntRect rect,
               enum class Direction direction = Direction(0),
               float speedX = 0.0f, float speedY = 0.0f,
               enum class GameObjectType onwer = GameObjectType(0));
        ~Bullet() override {}

        friend class Tank;

		void render(sf::RenderWindow* rw) override;

		void intersect(class GameObject* object) override;

		enum class GameObjectType getOwnerType() const { return _ownerType; }

    private :
        void setTextureRect(sf::IntRect rect) override;

        void setOwnerType(enum class GameObjectType owner) { _ownerType = owner; }

    private :
		enum class GameObjectType _ownerType;
};