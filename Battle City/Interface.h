#pragma once

#include "Level.h"


class Game;


/// <summary>
/// Класс, отвечающий за интерфейс игры
/// </summary>
class Interface
{
    public :
        Interface() = delete;
        explicit Interface(class Game&);

        /// <summary>
        /// Метод отрисовки показателей здоровья и остальной статистики игры
        /// </summary>
        void renderStats();
        /// <summary>
        /// Метод отрисовки миникарты
        /// </summary>
        void renderMap();
        /// <summary>
        /// Метод отсчёта времени до следующего обновления миникарты
        /// </summary>
        /// <param name="dt">- дельта времени,
        /// потраченного на прошлый игровой такт</param>
        void mapUpdate(float dt) { _mapUpdateTimer += dt; }

        Interface(const class Interface&) = delete;
        Interface operator=(const class Interface&) = delete;

    private :
        // Шрифт отладочной информации
        std::unique_ptr<sf::Font> _debugFont;
        // Шрифт сводочных данных о состоянии ключевых объектов
        std::unique_ptr<sf::Font> _objectsDataFont;

        // Слой, на который накладываются примитивы, обозначающие игровые объекты,
        // с целью дальнейшей отрисовки на миникарте
        std::unique_ptr<sf::RenderTexture> _mapLayout;
        sf::Sprite _map;  // Миникарта
        sf::RectangleShape _mapFrame;  // Подложка под миникарту

        float _mapUpdateTimer{2.0f};  // Таймер до следующего обновления миникарты
        const float MAP_UPDATE_PERIOD{2.0f};  // Через какой промежуток времени обновляется миникарта

        // Ссылка на объект игровой логики, которому принадлежит интерфейс
        class Game& _game;
};

