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
        /// ����� ���������� ��������� �����, �������: 
        /// �������, �����������, �������� ������
        /// </summary>
        /// <param name="dt">- ������ �������, 
        /// ����������� �� ��������� �������� �������� �����</param>
        void update(float dt) override;

        /// <summary>
        /// ����� ��������� �����
        /// </summary>
        /// <param name="rw">- ��������� �� ������ �������� ����</param>
        void render(sf::RenderWindow* rw) override;
        /// <summary>
        /// ����� ��������� �������� ������� ����� �� ���������
        /// </summary>
        /// <param name="rt">- ��������� �� ����, �� ������� ��������������
        /// ��� ��������� �������� ��� ���������</param>
        void mapRender(sf::RenderTexture* rt) override;

        Tank(const Tank&) = delete;
        Tank operator=(const Tank&) = delete;

    protected :
        /// <summary>
        /// �����, �������� ����������� � ��������
        /// �������� �����, ����������� � �������� ��� �������
        /// <para>����� ��������� ����� ����������� ������ ����������� ��� ���������
        /// ���������� �������� ����� ��� ����������</para>
        /// </summary>
        /// <param name="direction">- ����������� �������� (�����/����/������/�����)</param>
        /// <param name="dt">- ������ �������, ����������� �� ������� ������� ����</param>
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


        /// <summary>
        /// ��������� ������� ����������� ������ �����
        /// </summary>
        /// <param name="cooldownTime">- ����� �����������</param>
        void setFireCooldownTime(float cooldownTime) { _fireCooldownTime = cooldownTime; }
        /// <summary>
        /// ��������� ������� ����������� ������ �����
        /// </summary>
        /// <returns>���������� ����� ����������� ������</returns>
        float getFireCooldownTime() { return _fireCooldownTime; }

        /// <summary>
        /// ��������� ������������ �������� �����
        /// </summary>
        /// <param name="maxSpeed">- ������������ �������� �����</param>
        void setMaxSpeed(float maxSpeed) { _maxSpeed = maxSpeed; }
        /// <summary>
        /// ��������� ������������ �������� �����
        /// </summary>
        /// <returns>���������� ������������ �������� �����</returns>
        float getMaxSpeed() { return _maxSpeed; }

        /// <summary>
        /// ��������� �������� ��������� �����
        /// </summary>
        /// <param name="speedup">- �������� �������� ��������� �����</param>
        void setSpeedup(float speedup) { _speedup = speedup; }
        /// <summary>
        /// ��������� �������� ��������� �����
        /// </summary>
        /// <returns>���������� �������� ��������� �����</returns>
        float getSpeedup() { return _speedup; }

        /// <summary>
        /// ��������� ������� �������� �����
        /// </summary>
        /// <param name="currentSpeed">- ��������������� �������� �����</param>
        void setCurrentSpeed(float currentSpeed) { _currentSpeed = currentSpeed; }
        /// <summary>
        /// ��������� ������� �������� �����
        /// </summary>
        /// <returns>���������� ������� �������� �����</returns>
        float getCurrentSpeed() { return _currentSpeed; }

        /// <summary>
        /// ��������� �������� ���������� �����
        /// </summary>
        /// <param name="brakingSpeed">- ��������������� �������� ����������</param>
        void setBrakingSpeed(float brakingSpeed) { _brakingSpeed = brakingSpeed; }
        /// <summary>
        /// ��������� �������� ���������� �����
        /// </summary>
        /// <returns>���������� �������� ���������� �����</returns>
        float getBrakingSpeed() { return _brakingSpeed; }

        /// <summary>
        /// ��������� �������� ������� ����� ������� � ������� ��������� �����
        /// </summary>
        /// <param name="offset">- ��������������� ������� �����
        /// ������� � �������</param>
        void setOffset(float offset) { _offset = offset; }
        /// <summary>
        /// ��������� �������� ������� ����� ������� � ������� ��������� �����
        /// </summary>
        /// <returns>���������� ������� �����
        /// ������� � ������� ��������� �����</returns>
        float getOffset() { return _offset; }

        /// <summary>
        /// ��������� �������: ��������� ���� ��� ���
        /// </summary>
        /// <returns>���������� true ���� ���� ���������� �������,
        /// false ���� ���</returns>
        bool getRotation() { return _rotation; }

        /// <summary>
        /// ��������� �������, �������������� �� ������� �����
        /// </summary>
        /// <param name="rotationTime">- ��������������� �����,
        /// �� ������� ���� ���������� �� 90 ��������</param>
        void setRotationTime(float rotationTime) { _rotationTime = rotationTime; }
        /// <summary>
        /// ��������� �������, �������������� �� ������� �����
        /// </summary>
        /// <returns>���������� �����, �� �������
        /// ���� ������ ����������� �� 90 ��������</returns>
        float getRotationTime() { return _rotationTime; }

    private:
        /// <summary>
        /// ����� ������ ����������� ������, ������� ����������� ����
        /// ����� ��������� ������� � ����� �� ������, ����
        /// ����� ��� ����������� ����� �� ������� �����
        /// </summary>
        /// <returns>���������� true ���� �������� ����
        /// � ���� ��� �������� ��������</returns>
        bool bypassObstruction();

        /// <summary>
        /// �������� ������� ����� �� 90 ��� 180 ��������
        /// </summary>
        /// <param name="dt">- ������ �������, ����������� �� ������� ������� ����</param>
        /// <returns>���� ������ ���������, ���������� true. 
        /// <para>��� ������ ������ ����������� �� ������
        /// �������� ����������� �������� �����, ���������� false</para></returns>
        bool spriteRotation(float dt);

        /// <summary>
        /// �����, �������������� ������� ����� �� 90 ��� 180 ��������
        /// <para>��� ������ ��������� ����� ����������� �������� �� 90 ��������,
        ///  ���������� ���������� ��������� ����� � ��� �������� ������������ �����</para>
        /// </summary>
        /// <param name="dt">- ������ �������, ������������ �� ���������� ������� ����</param>
        void rotation(float dt);

        /// <summary>
        /// ��������� �������� ������ ����� ����� �������.
        /// ��� ���� �������� �����, ��� ���� �������� ������.
        /// </summary>
        void renderTracksMoving();

        /// <summary>
        /// ����� ������� ��������� � ��������� ��������
        /// </summary>
        /// <param name="speed">- ��������, ������� ����� �������</param>
        /// <param name="dt">- ������ ������� ������ �������� �����</param>
        /// <returns>���������� ����������� ��������</returns>
        float setBrakingSpeed(float speed, float dt);

    private :
        /// <summary>����� ����������� ������</summary>
        float _fireCooldownTime{0.0f};
        /// <summary>������������ �������� �����</summary>
        float _maxSpeed{0.0f};
        /// <summary>�������� ��������� �����</summary>
        float _speedup{0.0f};
        /// <summary>������� �������� �����</summary>
        float _currentSpeed{0.0f};
        /// <summary>�������� �������� ��� ���������� �����</summary>
        float _brakingSpeed{0.0f};
        /// <summary>�������� ����� ������� � ������� �����
        /// ��� �������� ��������� ��������� ��� ��������</summary>
        float _offset{0.0f};

        /// <summary>���������� �� �������� ����� �� ������ ������ (true/false)</summary>
        bool _rotation{false};
        /// <summary>����� � ��������, �� ������� ���� ��������� �� 90 ��������</summary>
        float _rotationTime{0.0f};
        /// <summary>������� ������������� ���� ���������� ������� �����</summary>
        float _rotationAngle{0.0f};
        /// <summary>�����������, �� �������� ������ ������� ����� 
        /// (true - �� �������, false - ������)</summary>
        bool _rotationClockwise{true};

        /// <summary>������� ���������� ���������� � ������� ����� ������� ������</summary>
        float _currentTrackShift{0.0f};

        /// <summary>���������� ����������� �������� � ������ �������� �����.
        /// <para>��� �������� ����� ��������� � ������������ �����,
        /// �������������� � ������ ����������� ��������</para></summary>
        enum Direction _oldDirection{Direction::UP};
        /// <summary>����������� ������� �����.
        /// ������������� �������� �����������, ��������������� ������������� � ��������</summary>
        enum Direction _inertiaDirection{Direction::NONE};
};
