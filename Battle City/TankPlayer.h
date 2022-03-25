#pragma once

#include "Tank.h"

/// <summary>
///  ласс управл€емого игроком танка
/// </summary>
class TankPlayer : public Tank
{
    public :
		TankPlayer();
		~TankPlayer() override {}

		/// <summary>
		/// ћетод обновлени€ состо€ни€ танка игрока
		/// <para>ќтслеживаютс€ нажатые клавиши управлени€ танком
		/// дл€ перевода в соотвествующее состо€ние</para>
		/// </summary>
		/// <param name="dt">- дельта времени, затраченное
		/// на предыдущий игровой такт</param>
		void update(float dt) override;

		/// <summary>
		/// ћетод установки соотвествующего спрайта дл€ танка игрока
		/// </summary>
		/// <param name="rect">- пр€моугольник "вырезающий"
		/// определЄнный спрайт из атласа</param>
		virtual void setTextureRect(sf::IntRect rect) override;

		/// <summary>
		/// ћетод назначени€ определЄнных клавиш дл€ управлени€ танком
		/// </summary>
		/// <param name="keyLeft">- клавиша дл€ движени€ налево</param>
		/// <param name="keyRight">- клавиша дл€ движени€ направо</param>
		/// <param name="keyUp">- клавиша дл€ движени€ вверх</param>
		/// <param name="keyDown">- клавиша дл€ движени€ вниз</param>
		/// <param name="keyFire">- клавиша дл€ выстрела</param>
		void setKeys(sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight,
					 sf::Keyboard::Key keyUp, sf::Keyboard::Key keyDown,
					 sf::Keyboard::Key keyFire);

    private :
	    // Ќазначаемые клавиши управлени€ танка игроком
	    /////////////////////
	    sf::Keyboard::Key _keyLeft {sf::Keyboard::Key::Unknown};
		sf::Keyboard::Key _keyRight{sf::Keyboard::Key::Unknown};
		sf::Keyboard::Key _keyUp   {sf::Keyboard::Key::Unknown};
		sf::Keyboard::Key _keyDown {sf::Keyboard::Key::Unknown};
		sf::Keyboard::Key _keyFire {sf::Keyboard::Key::Unknown};
};