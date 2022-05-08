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
        ~Interface() = default;

        /// <summary>
        /// Метод отрисовки показателей здоровья и остальной статистики игры
        /// </summary>
        void renderStats();
        /// <summary>
        /// Метод отрисовки миникарты
        /// </summary>
        void renderMap();
        /// <summary>
        /// Метод, обновляющий таймеры интерфейса,
        /// которые отвечают за приостановку внесения
        /// изменений в различные элементы интерфейса
        /// </summary>
        /// <param name="dt">- дельта времени,
        /// затраченного на предыдущий игровой такт</param>
        void timersUpdate(float dt);
        /// <summary>
        /// Обновление указателей на основные игровые объекты,
        /// которые существуют на данный момент
        /// </summary>
        void updateGameObjectsPointers();

        Interface(const class Interface&) = delete;
        Interface operator=(const class Interface&) = delete;

    private :
        /// <summary>
        /// Метод отрисовки полосы здоровья базы
        /// </summary>
        void showBaseHealth();
        /// <summary>
        /// Метод отрисовки полосы здоровья танка игрока
        /// </summary>
        void showTankHealth();
        /// <summary>
        /// Метод отрисовки индикатора(полосы) перезарядки танка игрока
        /// </summary>
        void showTankRecharge();

    private :
        // Шрифт отладочной информации
        sf::Font _debugFont;
        // Шрифт сводочных данных о состоянии ключевых объектов
        sf::Font _objectsDataFont;

        // Слой, на который накладываются примитивы, обозначающие игровые объекты,
        // с целью дальнейшей отрисовки на миникарте
        std::unique_ptr<sf::RenderTexture> _mapLayout;
        sf::Sprite _map;  // Миникарта
        sf::RectangleShape _mapFrame;  // Подложка под миникарту

        float _mapUpdateTimer{2.0f};  // Таймер до следующего обновления миникарты
        const float MAP_UPDATE_PERIOD{2.0f};  // Через какой промежуток времени обновляется миникарта

        // Количество мерцаний индикатора при изменении очков прочности базы/игрока
        const int NUMBER_OF_BLINKS{3};
        const float BLINK_TIME{0.1f};  // Время одного мерцания

        ///////////////////////////////////////////////////
        // Полоса очков прочности базы
        sf::RectangleShape _baseHealthFrame;  // Подложка полосы здоровья базы
        sf::RectangleShape _baseHealthBar;    // Полоса здоровья базы
        sf::Text _baseHealthText;             // Подпись полосы здоровья базы
        sf::Color _baseHealthColor;           // Цвет контура и текста полосы здоровья базы
        float _deltaBaseHealthBar{0.0f};      // Длина части полосы, равная одному очку прочности базы
        unsigned _deltaBaseHealthColorShift{0u};  // Разница цвета контура и текста при изменении на одно очко прочности базы
        int _baseHealthState{0};              // Показатель очков прочности базы, сохранённый на стороне интерфейса
        float _baseHealthBlinkTimer{0.0f};    // Таймер, отвечающий за мерцание при изменении очков прочности базы

        ///////////////////////////////////////////////////
        // Полоса очков прочности танка игрока
        sf::RectangleShape _tankHealthFrame;  // Подложка полосы здоровья танка игрока
        sf::RectangleShape _tankHealthBar;    // Полоса здоровья танка игрока
        sf::Text _tankHealthText;             // Подпись полосы здоровья танка игрока
        sf::Color _tankHealthColor;           // Цвет контура и текста полосы здоровья танка игрока
        float _deltaTankHealthBar{0.0f};      // Длина части полосы, равная одному очку прочности танка игрока
        unsigned _deltaTankHealthColorShift{0u};  // Разница цвета контура и текста при изменении на одно очко прочности танка
        int _tankHealthState{0};              // Показатель очков прочности танка игрока, сохранённый на стороне интерфейса
        float _tankHealthBlinkTimer{0.0f};    // Таймер, отвечающий за мерцание при изменении очков прочности танка игрока

        ///////////////////////////////////////////////////
        // Полоса индикатора перезарядки танка игрока
        sf::RectangleShape _rechargeFrame;  // Подложка индикатора перезарядки танка игрока
        sf::RectangleShape _rechargeBar;    // Полоса индикатора перезарядки танка игрока
        float _deltaRechargeBar{0.0f};      // Часть длины индикатора перезарядки, равная отрезку времени обновления индикатора
        const float RECHARGE_BAR_UPDATE_PERIOD{0.25f};  // Время обновления индикатора перезарядки
        float _rechargeBarUpdateTimer{0.0f};  // Таймер, отсчитывающий время до следующего обновления индикатора перезарядки

        // Ссылка на объект игровой логики, которому принадлежит интерфейс
        class Game& _game;

        class TankPlayer* _tankPlayer{nullptr};  // Указатель на объект танка игрока
        class Base* _base{nullptr};  // Указатель на объект базы
};

