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

    // ����� ������� ����� ������ ������
	if (_spawnTimer >= _spawnTime) {
		_spawnTimer = 0.0f;

        // ������� ���������� ����������� �� ������
		int enemiesOnLevel = getGame().getObjectsCount(GameObjectType::TANK_ENEMY);
        // ���������� ����� ��������� ���������� ����������� �� ������
		int enemiesStorageLeft = ENEMIES_PER_LEVEL - enemiesOnLevel
                                 - getGame().getDiedEnemiesCount();

		if (enemiesStorageLeft > 0
            && enemiesOnLevel < ENEMIES_PER_LEVEL_IN_ONE_MOMENT)
            // �������� ������� ����� ����������
			getGame().createObject(GameObjectType::TANK_ENEMY, getX(), getY());
	}
}