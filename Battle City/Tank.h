#pragma once

#include "GameObject.h"

/// <summary>
/// Базовый класс для всех игровых танков.
/// <para>Содержит основные расчёты
/// (скорости, выстрела, поворота, направления и т.д.)</para>
/// </summary>
class Tank : public GameObject
{
    public :
        explicit Tank(const class Game& game);
        ~Tank() override {}

        /// <summary>
        /// Обновление базовых параметров танка
        /// </summary>
        /// <param name="dt">- дельта времени одного игрового такта</param>
        void update(float dt) override;

        void render(sf::RenderWindow* rw) override;

        Tank(const Tank&) = delete;
        Tank operator=(const Tank&) = delete;

    protected :
        /// <summary>
        /// Метод, задающий направление и скорость
        /// движения танка
        /// </summary>
        /// <param name="direction">- направление движения (вверх/вниз/вправо/влево)</param>
        /// <param name="dt">- дельта времени одного игрового такта</param>
        void move(enum Direction direction, float dt);

        /// <summary>
        /// Метод, реализующий выстрел танка с созданием
        /// экземпляра соответствующего снаряда
        /// </summary>
        void fire();

        /// <summary>
        /// Получение координат дула орудия танка
        /// </summary>
        /// <param name="x">- координата по горизонтали</param>
        /// <param name="y">- координата по вертикали</param>
        /// <returns></returns>
        void calculateFrontCellPosition(float& x, float& y);

        ///////////////////////////////////////////////////////////////////////////////////////////
        // Сеттеры/Геттеры

        void setFireCooldownTime(float cooldownTime) { _fireCooldownTime = cooldownTime; }
        float getFireCooldownTime() { return _fireCooldownTime; }

        void setMaxSpeed(float maxSpeed) { _maxSpeed = maxSpeed; }
        float getMaxSpeed() { return _maxSpeed; }

        void setSpeedup(float speedup) { _speedup = speedup; }
        float getSpeedup() { return _speedup; }

        void setCurrentSpeed(float currentSpeed) { _currentSpeed = currentSpeed; }
        float getCurrentSpeed() { return _currentSpeed; }

        void setBrakingSpeed(float brakingSpeed) { _brakingSpeed = brakingSpeed; }
        float getBrakingSpeed() { return _brakingSpeed; }

        void setOffset(float offset) { _offset = offset; }
        float getOffset() { return _offset; }

        void setRotation(bool rotation) { _rotation = rotation; }
        bool getRotation() { return _rotation; }

        void setRotationTime(float rotationTime) { _rotationTime = rotationTime; }
        float getRotationTime() { return _rotationTime; }

        void setRotationAngle(float rotationAngle) { _rotationAngle = rotationAngle; }
        float getRotationAngle() { return _rotationAngle; }

        void setOldDirection(enum Direction oldDirection) { _oldDirection = oldDirection; }
        enum Direction getOldDirection() { return _oldDirection; }

    private:
        /// <summary>
        /// Метод уклонения от боковых препятствий на пути танка
        /// </summary>
        /// <returns>Возвращает True если одна из гусениц упирается в преграду перед танком</returns>
        bool bypassObstruction();

        /// <summary>
        /// Вращение спрайта танка на 90 или 180 градусов
        /// </summary>
        /// <param name="dt">- дельта времени одного игрового такта</param>
        /// <returns>Если происходит разворот танка возвращает True.
        /// Если танк уже развёрнут в нужном направлении или
        /// только закончил разворот, возвращает False</returns>
        bool rotation(float dt);

        /// <summary>
        /// Отрисовка вращения траков путем смены спрайта.
        /// Чем выше скорость танка, тем чаще меняется спрайт.
        /// </summary>
        void renderTracksMoving();

        ///////////////////////////////////////////////////////////////////////////////////////////
        // Методы для торможения танка по инерции

        /// <summary>
        /// Торможение по горизонтальной плоскости
        /// </summary>
        /// <param name="xSpeed">- скорость по горизонтальной плоскости</param>
        /// <param name="direction">- направление движения (лево/право)</param>
        /// <param name="dt">- дельта времени одного игрового такта</param>
        /// <returns>Возвращает уменьшенную скорость по X</returns>
        float setBrakingSpeed(float xSpeed, float dt);
        //
        /// <summary>
        /// Торможение по вертикальной плоскости
        /// </summary>
        /// <param name="xSpeed">- скорость по горизонтальной плоскости</param>
        /// <param name="direction">- направление движения (вверх/вниз)</param>
        /// <param name="dt">- дельта времени одного игрового такта</param>
        /// <returns>Возвращает уменьшенную скорость по Y</returns>
        //float setBrakingSpeedY(float ySpeed, float dt);

    private :
        /// <summary>Время перезарядки орудия</summary>
        float _fireCooldownTime{0.0f};
        /// <summary>Максимальная скорость танка</summary>
        float _maxSpeed{0.0f};
        /// <summary>Начальная скорость при ускорении танка</summary>
        float _speedup{0.0f};
        /// <summary>Текущая скорость танка</summary>
        float _currentSpeed{0.0f};
        /// <summary>Обратная скорость при торможении танка</summary>
        float _brakingSpeed{0.0f};
        /// <summary>Разность между шириной и высотой танка
        /// для удобного пересчёта габаритов при повороте</summary>
        float _offset{0.0f};

        /// <summary>Происходит ли вращение танка на данный момент (True/false)</summary>
        bool _rotation{false};
        /// <summary>Время в секундах, за которое танк повернётся на 90 градусов</summary>
        float _rotationTime{0.0f};
        /// <summary>Текущий установленный угол ориентации танка</summary>
        float _rotationAngle{0.0f};
        /// <summary>Текущее затраченное время на поворот танка</summary>
        float _currentRotationTime{0.0f};

        /// <summary>Текущее пройденное расстояние с прошлой смены спрайта траков</summary>
        float _currentTrackShift{0.0f};

        float _prevX{0.0f};  // Позиция по горизонтали на предыдущем такте
        float _prevY{0.0f};  // Позиция по вертикали на предыдущем такте

        /// <summary>Предыдущее направление движения до поворота танка.
        /// После поворота приравнивается к текущему направлению движения</summary>
        enum Direction _oldDirection{Direction::UP};
        /// <summary>Направление инерции танка.
        /// Соответствует прошлому направлению, перпендикулярно направленному к текущему</summary>
        enum Direction _inertiaDirection{Direction::NONE};
};
