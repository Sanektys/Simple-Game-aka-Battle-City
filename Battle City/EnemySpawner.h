#pragma once

#include "GameObject.h"


class EnemySpawner : public GameObject
{
    public :
        EnemySpawner();

        void update(float dt) override;

    private :
        float _spawnTimer;
        float _spawnTime;
};