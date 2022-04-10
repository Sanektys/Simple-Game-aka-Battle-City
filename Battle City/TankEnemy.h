#pragma once

#include "Tank.h"


/// <summary>
/// Класс базового танка противника
/// </summary>
class TankEnemy : public Tank
{
    public:
        TankEnemy(const class Game& game, sf::IntRect rect);
        ~TankEnemy() override;

        /// <summary>
        /// Метод обновления состояния экземпляра объекта танка противника
        /// </summary>
        /// <param name="dt">- дельта времени, затраченное на предыдущий игровой такт</param>
        void update(float dt) override;

        TankEnemy(const TankEnemy&) = delete;
        TankEnemy operator=(const TankEnemy&) = delete;

    protected:
        /// <summary>
        /// Метод, реализующий алгоритм действий танка противника
        /// </summary>
        /// <param name="dt">- дельта времени, затраченное на предыдущий игровой такт</param>
        void ai(float dt);

        /// <summary>
        /// Метод проверки на столкновение с препятствием на пути танка.
        /// <para>Меняет направление танка, если оно заслоняет
        /// больше трети передней части его корпуса</para>
        /// </summary>
        /// <param name="dt">- дельта времени, затраченное на предыдущий игровой такт</param>
        /// <returns>Возращает true, если танк столкнулся с препятствием,
        /// false - если нет</returns>
        bool isAvoidSolidBarrier(float dt);
        /// <summary>
        /// Метод смены направления движения танка, на новое,
        /// не совпадающее ни с текущим, ни с предыдущим
        /// </summary>
        /// <param name="dt">- дельта времени, затраченное на предыдущий игровой такт</param>
        void changeDirection(float dt);

    private:
        // Временная пауза между сменами направления танка
        float _changeDirectionPauseTime{0.0f};
        // Предыдущее направление движения танка
        enum Direction _prevDirection{Direction(0)};
};