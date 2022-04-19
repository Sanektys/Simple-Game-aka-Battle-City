#pragma once

#include "GameObject.h"


class Bullet;


/// <summary>
/// Класс трассера снаряда
/// </summary>
class BulletTracer : public GameObject
{
    public :
        friend Bullet;

        explicit BulletTracer(const class Game&);

        /// <summary>
        /// Метод обновления состояния трассера от снаряда
        /// <para>Проверяется, когда должен удалиться трассер</para>
        /// </summary>
        /// <param name="dt">- дельта времени,
        /// затраченного на предыдущий игровой такт</param>
        void update(float dt) override;

        /// <summary>
        /// Метод отрисовки трассера(простого закрашенного прямоугольника)
        /// </summary>
        /// <param name="rw">- указатель на объект игрового окна</param>
        void render(class sf::RenderWindow* rw) override;

        /// <summary>
        /// Установка направления полёта трассера
        /// </summary>
        /// <param name="direction">- устанавливаемое направление полёта</param>
        void setDirection(enum Direction direction) override;

        /// <summary>
        /// Установка параметров фигуры прямоугольника трассера
        /// <para>(его цвет, габариты, размер)</para>
        /// </summary>
        void setShapeRect();

        BulletTracer(const BulletTracer&) = delete;
        BulletTracer operator=(const BulletTracer&) = delete;

    private :
        /// <summary>
        /// Установка градации трассера по прозрачности (от 1 до 3)
        /// </summary>
        /// <param name="gradation">- устанавливаемый
        /// уровень прозрачности(1-3)</param>
        void setGradation(short gradation) { _gradation = gradation; }

        /// <summary>
        /// Установка того, кто является объектом-владельцом трассера
        /// </summary>
        /// <param name="owner">- указатель на объект
        /// снаряда-владельца трассера</param>
        void setOwner(const class GameObject* owner) { _owner = owner; }

        /// <summary>
        /// Метод сообщает классу трассера о том,
        /// что объект снаряда-владельца уничтожен
        /// <para>(указатель на объект-владельца обнуляется)</para>
        /// </summary>
        void bulletDestroyed() { _owner = nullptr; }

    private :
        // Отрисовываемая прямоугольная форма трассера
        sf::RectangleShape _tracerShape;
        // Указатель на снаряд, что создал данный трассер
        const class GameObject* _owner{nullptr};

        static const float BASIC_WIDTH;   // Стандартная ширина трассера
        static const float BASIC_HEIGHT;  // Стандартная высота трассера

        float _finalCoordX{0.0f};  // Последняя известная координата снаряда по горизонтали
        float _finalCoordY{0.0f};  // Последняя известная координата снаряда по вертикали

        // Три степени 1-3 прозрачности трассера
        short _gradation{1};
};