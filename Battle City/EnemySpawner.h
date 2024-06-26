#pragma once

#include "GameObject.h"


/// <summary>
/// ����� �������� �����������
/// </summary>
class EnemySpawner : public GameObject
{
    public :
        EnemySpawner(const class Game& game);
        ~EnemySpawner() override {}

        /// <summary>
        /// ����� ���������� ��������� ��������, ���������� ������ �������
        /// �� ���������� ������ � ������ ������� ����� ����������
        /// </summary>
        /// <param name="dt">- �����, ��������� �� ���������� ����
        /// (����� ��������� ������ � ��������� �� ������� ���)</param>
        void update(float dt) override;

        EnemySpawner(const EnemySpawner&) = delete;
        EnemySpawner operator=(const EnemySpawner&) = delete;

    private :
        // ����� �������� ����� ��������� ����������� � �������� ��������
        float _spawnTime{0.0f};
        // ������� ������� ������
        float _spawnTimer{0.0f};
};