#pragma once

#include "GameObject.h"


/// <summary>
/// ����� ������� �����
/// <para>���������� - ����� ���� � ����� ����� ���������,
/// ���� ���������� ��� �����</para>
/// </summary>
class Wall : public GameObject
{
    public :
        Wall(const class Game& game, sf::IntRect rect,
             bool isInvulnerable = false);
        ~Wall() override {}

		/// <summary>
		/// ����� ������ ������ �� ��� ����� �������� ����� �� ������,
        /// � ����������� �� �������� �����, ��������������� �������������
		/// </summary>
		/// <param name="rect">- ������������� "����������" �����������
        /// ������ �� ������</param>
		//virtual void setTextureRect(sf::IntRect rect) override;
    
    private :
        Wall(const Wall&) = delete;
        Wall operator=(const Wall&) = delete;

    protected :
        // �������� ������� ����� (1-3)
        int _variation{0};
};