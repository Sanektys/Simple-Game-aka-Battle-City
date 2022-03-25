#pragma once

#include "Tank.h"


class TankPlayer : public Tank
{
    public :
		TankPlayer();
		~TankPlayer() override {}

		void update(float dt) override;

		virtual void setTextureRect(sf::IntRect rect) override;

		void setKeys(sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight,
					 sf::Keyboard::Key keyUp, sf::Keyboard::Key keyDown,
					 sf::Keyboard::Key keyFire);

    private :
	    // Назначаемые клавишы управления танка игроком
	    /////////////////////
	    sf::Keyboard::Key _keyLeft {sf::Keyboard::Key::Unknown};
		sf::Keyboard::Key _keyRight{sf::Keyboard::Key::Unknown};
		sf::Keyboard::Key _keyUp   {sf::Keyboard::Key::Unknown};
		sf::Keyboard::Key _keyDown {sf::Keyboard::Key::Unknown};
		sf::Keyboard::Key _keyFire {sf::Keyboard::Key::Unknown};
};