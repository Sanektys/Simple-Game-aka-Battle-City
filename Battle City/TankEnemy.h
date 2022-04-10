#pragma once

#include "Tank.h"


/// <summary>
/// ����� �������� ����� ����������
/// </summary>
class TankEnemy : public Tank
{
    public:
        TankEnemy(const class Game& game, sf::IntRect rect);
        ~TankEnemy() override;

        /// <summary>
        /// ����� ���������� ��������� ���������� ������� ����� ����������
        /// </summary>
        /// <param name="dt">- ������ �������, ����������� �� ���������� ������� ����</param>
        void update(float dt) override;

        TankEnemy(const TankEnemy&) = delete;
        TankEnemy operator=(const TankEnemy&) = delete;

    protected:
        /// <summary>
        /// �����, ����������� �������� �������� ����� ����������
        /// </summary>
        /// <param name="dt">- ������ �������, ����������� �� ���������� ������� ����</param>
        void ai(float dt);

        /// <summary>
        /// ����� �������� �� ������������ � ������������ �� ���� �����.
        /// <para>������ ����������� �����, ���� ��� ���������
        /// ������ ����� �������� ����� ��� �������</para>
        /// </summary>
        /// <param name="dt">- ������ �������, ����������� �� ���������� ������� ����</param>
        /// <returns>��������� true, ���� ���� ���������� � ������������,
        /// false - ���� ���</returns>
        bool isAvoidSolidBarrier(float dt);
        /// <summary>
        /// ����� ����� ����������� �������� �����, �� �����,
        /// �� ����������� �� � �������, �� � ����������
        /// </summary>
        /// <param name="dt">- ������ �������, ����������� �� ���������� ������� ����</param>
        void changeDirection(float dt);

    private:
        // ��������� ����� ����� ������� ����������� �����
        float _changeDirectionPauseTime{0.0f};
        // ���������� ����������� �������� �����
        enum Direction _prevDirection{Direction(0)};
};