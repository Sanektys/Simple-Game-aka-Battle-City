#pragma once

#include "GameObject.h"


class Bullet : public GameObject
{
    public :
		Bullet();
		~Bullet();

		virtual void render(sf::RenderWindow* rw) override;
		virtual void setTextureRect(sf::IntRect rect) override;

		virtual void intersect(class GameObject* object) override;

		void setOwnerType(enum GameObjectType ownerType) { _ownerType = ownerType; }
		enum GameObjectType getOwnerType() { return _ownerType; }

    private :
		enum GameObjectType _ownerType;
};