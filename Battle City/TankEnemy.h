#pragma once

#include "Tank.h"


class TankEnemy : public Tank
{
    public :
	    TankEnemy(const class Game& game, sf::IntRect rect);
		~TankEnemy() override;

		void update(float dt) override;

        TankEnemy(const TankEnemy&) = delete;
        TankEnemy operator=(const TankEnemy&) = delete;

    protected :
		void analize(float dt);
		void moveRandomDirection(float dt);

    private :
		float _analizeTimer;
		float _analizeTime;

		float _lastAnalizeX;
		float _lastAnalizeY;
};