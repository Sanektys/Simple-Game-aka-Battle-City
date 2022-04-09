#pragma once

#include "Level.h"


class Game;


/// <summary>
///  ласс, отвечающий за интерфейс игры
/// </summary>
class Interface
{
    public :
        Interface() = delete;
        explicit Interface(class Game&);

        /// <summary>
        /// ћетод отрисовки интерфейса игры
        /// </summary>
        void render();

        Interface(const class Interface&) = delete;
        Interface operator=(const class Interface&) = delete;

    private :
        // Ўрифт отладочной информации
        std::unique_ptr<sf::Font> _debugFont;
        // Ўрифт сводочных данных о состо€нии ключевых объектов
        std::unique_ptr<sf::Font> _objectsDataFont;

        // —сылка на объект игровой логики, которому принадлежит интерфейс
        class Game& _game;
};

