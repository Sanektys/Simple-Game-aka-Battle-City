#pragma once

#include "Tank.h"


class TankPlayer : public Tank
{
    public :
		TankPlayer();

		virtual void setTextureRect(sf::IntRect rect) override;

		void update(float dt) override;

		void setKeys(int keyLeft, int keyRight, int keyUp, int keyDown, int keyFire);

    private :
		int _keyLeft;
		int _keyRight;
		int _keyUp;
		int _keyDown;
		int _keyFire;
};