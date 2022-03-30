#pragma once

#include "GameObject.h"


class EnemySpawner : public GameObject
{
    public :
        EnemySpawner();
        ~EnemySpawner() override {}

        /// <summary>
        /// ����� ���������� ��������� ��������, ���������� ������ �������
        /// �� ���������� ������ � ������ ������� ����� ����������
        /// </summary>
        /// <param name="dt">- �����, ��������� �� ���������� ����
        /// (����� ��������� ������ � ��������� �� ������� ���)</param>
        void update(float dt) override;

    private :
        EnemySpawner(const EnemySpawner&) = delete;
        EnemySpawner operator=(const EnemySpawner&) = delete;

    private :
        // ����� �������� ����� ��������� ����������� � �������� ��������
        float _spawnTime{0.0f};
        // ������� ������� ������
        float _spawnTimer{0.0f};
};