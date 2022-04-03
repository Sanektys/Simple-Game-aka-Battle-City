#pragma once

#include "GameObject.h"
#include "Tank.h"


class Bullet : public GameObject
{
    public :
        friend class Tank;

        Bullet(const class Game& game, sf::IntRect rect,
               enum class Direction direction = Direction(0),
               float speedX = 0.0f, float speedY = 0.0f,
               enum class GameObjectType onwer = GameObjectType(0));
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
		enum class GameObjectType getOwnerType() const { return _ownerType; }

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
        void setDirection(enum class Direction direction) override;

        /// <summary>
        /// ��������� ���� �������, ������� �������� ������ ������
        /// </summary>
        /// <param name="owner">- ��� �������-������� �������
        /// ���������� �������</param>
        void setOwnerType(enum class GameObjectType owner) { _ownerType = owner; }

    private :
       // ��� �������, ��� �������� ������ ������
		enum class GameObjectType _ownerType;
};