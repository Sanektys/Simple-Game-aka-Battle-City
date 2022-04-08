#pragma once

#include "Tank.h"


class TankEnemy : public Tank
{
    public:
        TankEnemy(const class Game& game, sf::IntRect rect);
        ~TankEnemy() override;

        void update(float dt) override;

        TankEnemy(const TankEnemy&) = delete;
        TankEnemy operator=(const TankEnemy&) = delete;

    protected:
        void ai(float dt);

        bool isAvoidSolidBarrier(float dt);
        void changeDirection(float dt);

    private:
        float _changeDirectionPauseTime{0.0f};
        //float _leavingTargetPauseTime{0.0f};
};