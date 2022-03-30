#pragma once

#include "GameObject.h"


class EnemySpawner : public GameObject
{
    public :
        EnemySpawner();
        ~EnemySpawner() override {}

        /// <summary>
        /// Метод обновления состояния спавнера, производит отсчёт времени
        /// до следующего спавна и создаёт объекта танка противника
        /// </summary>
        /// <param name="dt">- время, прошедшее за предыдущий такт
        /// (время обработки логики и отрисовки за прошлый раз)</param>
        void update(float dt) override;

    private :
        EnemySpawner(const EnemySpawner&) = delete;
        EnemySpawner operator=(const EnemySpawner&) = delete;

    private :
        // Время задержки между созданием противников у текущего спавнера
        float _spawnTime{0.0f};
        // Счётчик времени спавна
        float _spawnTimer{0.0f};
};