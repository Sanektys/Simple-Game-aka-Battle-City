#pragma once

#include "GameObject.h"


class Bullet;


/// <summary>
/// ����� �������� �������
/// </summary>
class BulletTracer : public GameObject
{
    public :
        friend Bullet;

        explicit BulletTracer(const class Game&);

        /// <summary>
        /// ����� ���������� ��������� �������� �� �������
        /// <para>�����������, ����� ������ ��������� �������</para>
        /// </summary>
        /// <param name="dt">- ������ �������,
        /// ������������ �� ���������� ������� ����</param>
        void update(float dt) override;

        /// <summary>
        /// ����� ��������� ��������(�������� ������������ ��������������)
        /// </summary>
        /// <param name="rw">- ��������� �� ������ �������� ����</param>
        void render(class sf::RenderWindow* rw) override;

        /// <summary>
        /// ��������� ����������� ����� ��������
        /// </summary>
        /// <param name="direction">- ��������������� ����������� �����</param>
        void setDirection(enum Direction direction) override;

        /// <summary>
        /// ��������� ���������� ������ �������������� ��������
        /// <para>(��� ����, ��������, ������)</para>
        /// </summary>
        void setShapeRect();

        BulletTracer(const BulletTracer&) = delete;
        BulletTracer operator=(const BulletTracer&) = delete;

    private :
        /// <summary>
        /// ��������� �������� �������� �� ������������ (�� 1 �� 3)
        /// </summary>
        /// <param name="gradation">- ���������������
        /// ������� ������������(1-3)</param>
        void setGradation(short gradation) { _gradation = gradation; }

        /// <summary>
        /// ��������� ����, ��� �������� ��������-���������� ��������
        /// </summary>
        /// <param name="owner">- ��������� �� ������
        /// �������-��������� ��������</param>
        void setOwner(const class GameObject* owner) { _owner = owner; }

        /// <summary>
        /// ����� �������� ������ �������� � ���,
        /// ��� ������ �������-��������� ���������
        /// <para>(��������� �� ������-��������� ����������)</para>
        /// </summary>
        void bulletDestroyed() { _owner = nullptr; }

    private :
        // �������������� ������������� ����� ��������
        sf::RectangleShape _tracerShape;
        // ��������� �� ������, ��� ������ ������ �������
        const class GameObject* _owner{nullptr};

        static const float BASIC_WIDTH;   // ����������� ������ ��������
        static const float BASIC_HEIGHT;  // ����������� ������ ��������

        float _finalCoordX{0.0f};  // ��������� ��������� ���������� ������� �� �����������
        float _finalCoordY{0.0f};  // ��������� ��������� ���������� ������� �� ���������

        // ��� ������� 1-3 ������������ ��������
        short _gradation{1};
};