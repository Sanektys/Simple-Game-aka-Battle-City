#pragma once

#include "GameObject.h"
#include "Tank.h"


class Bullet : public GameObject
{
    public :
        friend class Tank;

        Bullet(const class Game& game, sf::IntRect rect,
               enum Direction direction = Direction(0),
               float speedX = 0.0f, float speedY = 0.0f,
               enum GameObjectType onwer = GameObjectType(0));
        ~Bullet() override {}

        /// <summary>
        /// �����, ������������ ��������� ������� ������� ��� ��������
        /// � ������ ��������
        /// </summary>
        /// <param name="object">- ������,
        /// � ������� ��������� ������������/�����������</param>
        void intersect(class GameObject* object) override;

        /// <summary>
        /// ��������� ���� �������, ������� �������� ������ ������
        /// </summary>
        /// <returns>���������� ��� ������-�������
        ///  ������� ���������� �������</returns>
        enum GameObjectType getOwnerType() const { return _ownerType; }

        Bullet(const Bullet&) = delete;
        Bullet operator=(const Bullet&) = delete;

    private :
        /// <summary>
        /// ��������� ������� ������� �������� ���� ������� �
        /// ������� �������� ����������� �����
        /// </summary>
        /// <param name="rect">- �������������, "����������" ������ ������
        /// �� ������ ��������</param>
        void setTextureRect(sf::IntRect rect) override;

        /// <summary>
        /// ��������� ����������� ����� �������
        /// � �������� ��� ����������� ��������� ���� �� ����� �� �����������
        /// </summary>
        /// <param name="direction">- ����������� ����� �������</param>
        void setDirection(enum Direction direction) override;

        /// <summary>
        /// ��������� ���� �������, ������� �������� ������ ������
        /// </summary>
        /// <param name="owner">- ��� �������-������� �������
        /// ���������� �������</param>
        void setOwnerType(enum GameObjectType owner) { _ownerType = owner; }

    private :
        // ��� �������, ��� �������� ������ ������
        enum GameObjectType _ownerType;
};