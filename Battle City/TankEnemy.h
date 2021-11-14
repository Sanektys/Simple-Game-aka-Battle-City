#pragma once

#include "Tank.h"


class TankEnemy : public Tank
{
    public :
	    TankEnemy();
		~TankEnemy();

		virtual void setTextureRect(sf::IntRect rect) override;

		void update(float dt) override;

    protected :
		void analize(float dt);
		void moveRandomDirection(float dt);

    private :
		float _analizeTimer;
		float _analizeTime;

		float _lastAnalizeX;
		float _lastAnalizeY;
};