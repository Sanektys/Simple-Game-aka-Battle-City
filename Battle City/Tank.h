#pragma once

#include "GameObject.h"

/// <summary>
/// ������� ����� ��� ���� ������� ������.
/// <para>�������� �������� �������
/// (��������, ��������, ��������, ����������� � �.�.)</para>
/// </summary>
class Tank : public GameObject
{
    public :
        explicit Tank(const class Game& game);
        ~Tank() override {}

        /// <summary>
        /// ���������� ������� ���������� �����
        /// </summary>
        /// <param name="dt">- ������ ������� ������ �������� �����</param>
        void update(float dt) override;

        void render(sf::RenderWindow* rw) override;

        Tank(const Tank&) = delete;
        Tank operator=(const Tank&) = delete;

    protected :
        /// <summary>
        /// �����, �������� ����������� � ��������
        /// �������� �����
        /// </summary>
        /// <param name="direction">- ����������� �������� (�����/����/������/�����)</param>
        /// <param name="dt">- ������ ������� ������ �������� �����</param>
        void move(enum Direction direction, float dt);

        /// <summary>
        /// �����, ����������� ������� ����� � ���������
        /// ���������� ���������������� �������
        /// </summary>
        void fire();

        /// <summary>
        /// ��������� ��������� ���� ������ �����
        /// </summary>
        /// <param name="x">- ���������� �� �����������</param>
        /// <param name="y">- ���������� �� ���������</param>
        /// <returns></returns>
        void calculateFrontCellPosition(float& x, float& y);

        ///////////////////////////////////////////////////////////////////////////////////////////
        // �������/�������

        void setFireCooldownTime(float cooldownTime) { _fireCooldownTime = cooldownTime; }
        float getFireCooldownTime() { return _fireCooldownTime; }

        void setMaxSpeed(float maxSpeed) { _maxSpeed = maxSpeed; }
        float getMaxSpeed() { return _maxSpeed; }

        void setSpeedup(float speedup) { _speedup = speedup; }
        float getSpeedup() { return _speedup; }

        void setCurrentSpeed(float currentSpeed) { _currentSpeed = currentSpeed; }
        float getCurrentSpeed() { return _currentSpeed; }

        void setBrakingSpeed(float brakingSpeed) { _brakingSpeed = brakingSpeed; }
        float getBrakingSpeed() { return _brakingSpeed; }

        void setOffset(float offset) { _offset = offset; }
        float getOffset() { return _offset; }

        void setRotation(bool rotation) { _rotation = rotation; }
        bool getRotation() { return _rotation; }

        void setRotationTime(float rotationTime) { _rotationTime = rotationTime; }
        float getRotationTime() { return _rotationTime; }

        void setRotationAngle(float rotationAngle) { _rotationAngle = rotationAngle; }
        float getRotationAngle() { return _rotationAngle; }

        void setOldDirection(enum Direction oldDirection) { _oldDirection = oldDirection; }
        enum Direction getOldDirection() { return _oldDirection; }

    private:
        /// <summary>
        /// ����� ��������� �� ������� ����������� �� ���� �����
        /// </summary>
        /// <returns>���������� True ���� ���� �� ������� ��������� � �������� ����� ������</returns>
        bool bypassObstruction();

        /// <summary>
        /// �������� ������� ����� �� 90 ��� 180 ��������
        /// </summary>
        /// <param name="dt">- ������ ������� ������ �������� �����</param>
        /// <returns>���� ���������� �������� ����� ���������� True.
        /// ���� ���� ��� �������� � ������ ����������� ���
        /// ������ �������� ��������, ���������� False</returns>
        bool rotation(float dt);

        /// <summary>
        /// ��������� �������� ������ ����� ����� �������.
        /// ��� ���� �������� �����, ��� ���� �������� ������.
        /// </summary>
        void renderTracksMoving();

        ///////////////////////////////////////////////////////////////////////////////////////////
        // ������ ��� ���������� ����� �� �������

        /// <summary>
        /// ���������� �� �������������� ���������
        /// </summary>
        /// <param name="xSpeed">- �������� �� �������������� ���������</param>
        /// <param name="direction">- ����������� �������� (����/�����)</param>
        /// <param name="dt">- ������ ������� ������ �������� �����</param>
        /// <returns>���������� ����������� �������� �� X</returns>
        float setBrakingSpeed(float xSpeed, float dt);
        //
        /// <summary>
        /// ���������� �� ������������ ���������
        /// </summary>
        /// <param name="xSpeed">- �������� �� �������������� ���������</param>
        /// <param name="direction">- ����������� �������� (�����/����)</param>
        /// <param name="dt">- ������ ������� ������ �������� �����</param>
        /// <returns>���������� ����������� �������� �� Y</returns>
        //float setBrakingSpeedY(float ySpeed, float dt);

    private :
        /// <summary>����� ����������� ������</summary>
        float _fireCooldownTime{0.0f};
        /// <summary>������������ �������� �����</summary>
        float _maxSpeed{0.0f};
        /// <summary>��������� �������� ��� ��������� �����</summary>
        float _speedup{0.0f};
        /// <summary>������� �������� �����</summary>
        float _currentSpeed{0.0f};
        /// <summary>�������� �������� ��� ���������� �����</summary>
        float _brakingSpeed{0.0f};
        /// <summary>�������� ����� ������� � ������� �����
        /// ��� �������� ��������� ��������� ��� ��������</summary>
        float _offset{0.0f};

        /// <summary>���������� �� �������� ����� �� ������ ������ (True/false)</summary>
        bool _rotation{false};
        /// <summary>����� � ��������, �� ������� ���� ��������� �� 90 ��������</summary>
        float _rotationTime{0.0f};
        /// <summary>������� ������������� ���� ���������� �����</summary>
        float _rotationAngle{0.0f};
        /// <summary>������� ����������� ����� �� ������� �����</summary>
        float _currentRotationTime{0.0f};

        /// <summary>������� ���������� ���������� � ������� ����� ������� ������</summary>
        float _currentTrackShift{0.0f};

        float _prevX{0.0f};  // ������� �� ����������� �� ���������� �����
        float _prevY{0.0f};  // ������� �� ��������� �� ���������� �����

        /// <summary>���������� ����������� �������� �� �������� �����.
        /// ����� �������� �������������� � �������� ����������� ��������</summary>
        enum Direction _oldDirection{Direction::UP};
        /// <summary>����������� ������� �����.
        /// ������������� �������� �����������, ��������������� ������������� � ��������</summary>
        enum Direction _inertiaDirection{Direction::NONE};
};
