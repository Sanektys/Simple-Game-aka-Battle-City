#include "EnemySpawner.h"
#include "GameObjectType.h"
#include "TankEnemy.h"
#include "Utils.h"
#include "Game.h"


EnemySpawner::EnemySpawner() {
	_type = GameObjectType::ENEMY_SPAWNER;

	_spawnTime = getRandomFloat(5.0f, ENEMY_SPAWNER_SPAWN_TIME);

	setPhysical(false);
}

void EnemySpawner::update(float dt) {
	_spawnTimer += dt;

    // ����� ������� ����� ������ ������
	if (_spawnTimer >= _spawnTime) {
		_spawnTimer = 0.0f;

        // ������� ���������� ����������� �� ������
		int enemiesOnLevel = _game->getObjectsCount(GameObjectType::TANK_ENEMY);
        // ���������� ����� ��������� ���������� ����������� �� ������
		int enemiesStorageLeft = ENEMIES_PER_LEVEL - enemiesOnLevel
                                 - _game->getDiedEnemiesCount();

		if (enemiesStorageLeft > 0
            && enemiesOnLevel < ENEMIES_PER_LEVEL_IN_ONE_MOMENT) {
            // �������� ������� ����� ����������
			GameObject* tankEnemy = _game->createObject(
                GameObjectType::TANK_ENEMY, getX(), getY());
			if (tankEnemy)
			    tankEnemy->setTextureRect(BASIC_ENEMY_TANK_IMAGE);
		}
	}
}