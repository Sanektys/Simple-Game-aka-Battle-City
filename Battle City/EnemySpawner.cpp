#include "EnemySpawner.h"
#include "GameObjectType.h"
#include "TankEnemy.h"
#include "Utils.h"
#include "Game.h"


EnemySpawner::EnemySpawner(const class Game& game) : GameObject(game) {
    setGroup(GameObjectGroup::ENTITY);
	setType(GameObjectType::ENEMY_SPAWNER);

	_spawnTime = getRandomFloat(5.0f, ENEMY_SPAWNER_SPAWN_TIME);

	setPhysical(false);
}

void EnemySpawner::update(float dt) {
	_spawnTimer += dt;

    // Когда подошло время нового спавна
	if (_spawnTimer >= _spawnTime) {
		_spawnTimer = 0.0f;

        // Текущее количество противников на уровне
		int enemiesOnLevel = getGame().getObjectsCount(GameObjectType::TANK_ENEMY);
        // Оставшееся общее возможное количество противников на уровне
		int enemiesStorageLeft = ENEMIES_PER_LEVEL - enemiesOnLevel
                                 - getGame().getDiedEnemiesCount();

		if (enemiesStorageLeft > 0
            && enemiesOnLevel < ENEMIES_PER_LEVEL_IN_ONE_MOMENT)
            // Создание объекта танка противника
			getGame().createObject(GameObjectType::TANK_ENEMY, getX(), getY());
	}
}