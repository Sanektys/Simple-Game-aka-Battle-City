#pragma once

#include "Tank.h"


/// <summary>
/// Класс управляемого игроком танка
/// </summary>
class TankPlayer : public Tank
{
    public :
        TankPlayer(const class Game& game, sf::IntRect rect,
                   sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight,
                   sf::Keyboard::Key keyUp,   sf::Keyboard::Key keyDown,
                   sf::Keyboard::Key keyFire);
        ~TankPlayer() override {}

        /// <summary>
        /// Метод обновления состояния танка игрока
        /// <para>Отслеживаются нажатые клавиши управления танком
        /// для перевода в соотвествующее состояние</para>
        /// </summary>
        /// <param name="dt">- дельта времени, затраченное
        /// на предыдущий игровой такт</param>
        void update(float dt) override;

        /// <summary>
        /// Метод назначения основных клавиш для управления танком
        /// </summary>
        /// <param name="keyLeft">- клавиша для движения налево</param>
        /// <param name="keyRight">- клавиша для движения направо</param>
        /// <param name="keyUp">- клавиша для движения вверх</param>
        /// <param name="keyDown">- клавиша для движения вниз</param>
        /// <param name="keyFire">- клавиша для выстрела</param>
        void setKeys(sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight,
                     sf::Keyboard::Key keyUp,   sf::Keyboard::Key keyDown,
                     sf::Keyboard::Key keyFire);
        /// <summary>
        /// Метод назначения альтернативных клавиш для управления танком
        /// </summary>
        /// <param name="keyLeft">- клавиша для движения налево</param>
        /// <param name="keyRight">- клавиша для движения направо</param>
        /// <param name="keyUp">- клавиша для движения вверх</param>
        /// <param name="keyDown">- клавиша для движения вниз</param>
        /// <param name="keyFire">- клавиша для выстрела</param>
        void setAltKeys(sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight,
                        sf::Keyboard::Key keyUp,   sf::Keyboard::Key keyDown,
                        sf::Keyboard::Key keyFire);

        TankPlayer(const TankPlayer&) = delete;
        TankPlayer operator=(const TankPlayer&) = delete;

    private :
        // Назначаемые клавиши управления танка игроком
        /////////////////////
        sf::Keyboard::Key _keyLeft {sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _keyRight{sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _keyUp   {sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _keyDown {sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _keyFire {sf::Keyboard::Key::Unknown};
        /////////////////////
        sf::Keyboard::Key _altKeyLeft {sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _altKeyRight{sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _altKeyUp   {sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _altKeyDown {sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _altKeyFire {sf::Keyboard::Key::Unknown};
};