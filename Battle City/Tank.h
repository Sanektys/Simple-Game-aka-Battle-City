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
        /// Метод обновления состояния танка, включая: 
        /// поворот, перезарядку, анимацию траков
        /// </summary>
        /// <param name="dt">- дельта времени, 
        /// затраченное на обработку прошлого игрового такта</param>
        void update(float dt) override;

        /// <summary>
        /// Метод отрисовки танка
        /// </summary>
        /// <param name="rw">- указатель на объект игрового окна</param>
        void render(sf::RenderWindow* rw) override;
        /// <summary>
        /// Метод отрисовки круглого маркера танка на миникарте
        /// </summary>
        /// <param name="rt">- указатель на слой, на котором отрисовываются
        /// все примитивы объектов для миникарты</param>
        void mapRender(sf::RenderTexture* rt) override;

        Tank(const Tank&) = delete;
        Tank operator=(const Tank&) = delete;

    protected :
        /// <summary>
        /// Метод, задающий направление и скорость
        /// движения танка, направление и скорость его инерции
        /// <para>Также запускает метод прохождения сквозь препятствия или запрещает
        /// дальнейшее движение когда это необходимо</para>
        /// </summary>
        /// <param name="direction">- направление движения (вверх/вниз/вправо/влево)</param>
        /// <param name="dt">- дельта времени, затраченное на прошлый игровой такт</param>
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


        /// <summary>
        /// Установка времени перезарядки орудия танка
        /// </summary>
        /// <param name="cooldownTime">- время перезарядки</param>
        void setFireCooldownTime(float cooldownTime) { _fireCooldownTime = cooldownTime; }
        /// <summary>
        /// Получение времени перезарядки орудия танка
        /// </summary>
        /// <returns>Возвращает время перезарядки орудия</returns>
        float getFireCooldownTime() { return _fireCooldownTime; }

        /// <summary>
        /// Установка максимальной скорости танка
        /// </summary>
        /// <param name="maxSpeed">- максимальная скорость танка</param>
        void setMaxSpeed(float maxSpeed) { _maxSpeed = maxSpeed; }
        /// <summary>
        /// Получение максимальной скорости танка
        /// </summary>
        /// <returns>Возвращает максимальную скорость танка</returns>
        float getMaxSpeed() { return _maxSpeed; }

        /// <summary>
        /// Установка величины ускорения танка
        /// </summary>
        /// <param name="speedup">- значение величины ускорения танка</param>
        void setSpeedup(float speedup) { _speedup = speedup; }
        /// <summary>
        /// Получение величины ускорения танка
        /// </summary>
        /// <returns>Возвращает величину ускорения танка</returns>
        float getSpeedup() { return _speedup; }

        /// <summary>
        /// Установка текущей скорости танка
        /// </summary>
        /// <param name="currentSpeed">- устанавливаемая скорость танка</param>
        void setCurrentSpeed(float currentSpeed) { _currentSpeed = currentSpeed; }
        /// <summary>
        /// Получение текущей скорости танка
        /// </summary>
        /// <returns>Возвращает текущую скорость танка</returns>
        float getCurrentSpeed() { return _currentSpeed; }

        /// <summary>
        /// Установка скорости торможения танка
        /// </summary>
        /// <param name="brakingSpeed">- устанавливаемая скорость торможения</param>
        void setBrakingSpeed(float brakingSpeed) { _brakingSpeed = brakingSpeed; }
        /// <summary>
        /// Получение скорости торможения танка
        /// </summary>
        /// <returns>Возвращает скорость торможения танка</returns>
        float getBrakingSpeed() { return _brakingSpeed; }

        /// <summary>
        /// Установка половины разницы между шириной и высотой габаритов танка
        /// </summary>
        /// <param name="offset">- устанавливаемая разница между
        /// шириной и высотой</param>
        void setOffset(float offset) { _offset = offset; }
        /// <summary>
        /// Получение половины разницы между шириной и высотой габаритов танка
        /// </summary>
        /// <returns>Возвращает разницу между
        /// шириной и высотой габаритов танка</returns>
        float getOffset() { return _offset; }

        /// <summary>
        /// Получение статуса: вращается танк или нет
        /// </summary>
        /// <returns>Возвращает true если танк производит поворот,
        /// false если нет</returns>
        bool getRotation() { return _rotation; }

        /// <summary>
        /// Установка времени, затрачиваемого на поворот танка
        /// </summary>
        /// <param name="rotationTime">- устанавливаемое время,
        /// за которое танк повернется на 90 градусов</param>
        void setRotationTime(float rotationTime) { _rotationTime = rotationTime; }
        /// <summary>
        /// Получение времени, затрачиваемого на поворот танка
        /// </summary>
        /// <returns>Возвращает время, за которое
        /// танк должен повернуться на 90 градусов</returns>
        float getRotationTime() { return _rotationTime; }

    private:
        /// <summary>
        /// Метод обхода препятствий танком, которые расположены либо
        /// перед передними катками с одной из сторон, либо
        /// когда они расположены сбоку от заднего катка
        /// </summary>
        /// <returns>Возвращает true если преграда есть
        /// и танк его пытается миновать</returns>
        bool bypassObstruction();

        /// <summary>
        /// Вращение спрайта танка на 90 или 180 градусов
        /// </summary>
        /// <param name="dt">- дельта времени, затраченное на прошлый игровой такт</param>
        /// <returns>Пока спрайт вращается, возвращает true. 
        /// <para>Как только спрайт установился на градус
        /// текущего направления движения танка, возвращает false</para></returns>
        bool spriteRotation(float dt);

        /// <summary>
        /// Метод, обеспечивающий поворот танка на 90 или 180 градусов
        /// <para>Как только произошла смена направления движения на 90 градусов,
        ///  происходит обновление габаритов танка и его основной координатной точки</para>
        /// </summary>
        /// <param name="dt">- дельта времени, затраченного на предыдущий игровой такт</param>
        void rotation(float dt);

        /// <summary>
        /// Отрисовка движения траков путем смены спрайта.
        /// Чем выше скорость танка, тем чаще меняется спрайт.
        /// </summary>
        void renderTracksMoving();

        /// <summary>
        /// Метод снижает указанную в аргументе скорость
        /// </summary>
        /// <param name="speed">- скорость, которую нужно снизить</param>
        /// <param name="dt">- дельта времени одного игрового такта</param>
        /// <returns>Возвращает уменьшенную скорость</returns>
        float setBrakingSpeed(float speed, float dt);

    private :
        /// <summary>Время перезарядки орудия</summary>
        float _fireCooldownTime{0.0f};
        /// <summary>Максимальная скорость танка</summary>
        float _maxSpeed{0.0f};
        /// <summary>Величина ускорения танка</summary>
        float _speedup{0.0f};
        /// <summary>Текущая скорость танка</summary>
        float _currentSpeed{0.0f};
        /// <summary>Обратная скорость при торможении танка</summary>
        float _brakingSpeed{0.0f};
        /// <summary>Разность между шириной и высотой танка
        /// для удобного пересчёта габаритов при повороте</summary>
        float _offset{0.0f};

        /// <summary>Происходит ли вращение танка на данный момент (true/false)</summary>
        bool _rotation{false};
        /// <summary>Время в секундах, за которое танк повернётся на 90 градусов</summary>
        float _rotationTime{0.0f};
        /// <summary>Текущий установленный угол ориентации спрайта танка</summary>
        float _rotationAngle{0.0f};
        /// <summary>Направление, по которому ведётся поворот танка 
        /// (true - по часовой, false - против)</summary>
        bool _rotationClockwise{true};

        /// <summary>Текущее пройденное расстояние с прошлой смены спрайта траков</summary>
        float _currentTrackShift{0.0f};

        /// <summary>Предыдущее направление движения к началу поворота танка.
        /// <para>При успешной смене габаритов и координатной точки,
        /// приравнивается к новому направлению движения</para></summary>
        enum Direction _oldDirection{Direction::UP};
        /// <summary>Направление инерции танка.
        /// Соответствует прошлому направлению, перпендикулярно направленному к текущему</summary>
        enum Direction _inertiaDirection{Direction::NONE};
};
