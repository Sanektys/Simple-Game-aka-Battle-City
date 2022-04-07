#pragma once

#include "GameObject.h"


/// <summary>
/// Класс игровой базы
/// </summary>
class Base : public GameObject
{
    public :
        Base(const class Game&, sf::IntRect);
        ~Base() override {}

        /// <summary>
        /// Метод отрисовки спрайта игровой базы
        /// </summary>
        /// <param name="rw">- указатель на объект игрового окна</param>
        virtual void render(sf::RenderWindow* rw) override;
        /// <summary>
        /// Метод обновления состояния объекта игровой базы
        /// </summary>
        /// <param name="dt">- дельта времени,
        /// затраченного на предыдущий игровой такт</param>
        virtual void update(float dt) override;

        Base(const Base&) = delete;
        Base operator=(const Base&) = delete;

    private :
        /// <summary>
        /// Обновление спрайта базы в зависимости от оставшихся очков прочности
        /// <para>Метод также обеспечивает анимацию огня при сильных
        /// повреждениях игровой базы</para>
        /// </summary>
        /// <param name="rect">- прямоугольник "вырезающий" определённый спрайт
        /// из всего атласа</param>
        virtual void setTextureRect(sf::IntRect rect) override;

    private :
        // Переменные для смены спрайта базы
        ////////////////	
        float _frameSwitchTime{0.0f};
        short _horizontalFrameNumber{1};
        bool _toLeft{true};
};