#pragma once

#include "Tank.h"

/// <summary>
/// ����� ������������ ������� �����
/// </summary>
class TankPlayer : public Tank
{
    public :
		TankPlayer();
		~TankPlayer() override {}

		/// <summary>
		/// ����� ���������� ��������� ����� ������
		/// <para>������������� ������� ������� ���������� ������
		/// ��� �������� � �������������� ���������</para>
		/// </summary>
		/// <param name="dt">- ������ �������, �����������
		/// �� ���������� ������� ����</param>
		void update(float dt) override;

		/// <summary>
		/// ����� ��������� ��������������� ������� ��� ����� ������
		/// </summary>
		/// <param name="rect">- ������������� "����������"
		/// ����������� ������ �� ������</param>
		virtual void setTextureRect(sf::IntRect rect) override;

		/// <summary>
		/// ����� ���������� ����������� ������ ��� ���������� ������
		/// </summary>
		/// <param name="keyLeft">- ������� ��� �������� ������</param>
		/// <param name="keyRight">- ������� ��� �������� �������</param>
		/// <param name="keyUp">- ������� ��� �������� �����</param>
		/// <param name="keyDown">- ������� ��� �������� ����</param>
		/// <param name="keyFire">- ������� ��� ��������</param>
		void setKeys(sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight,
					 sf::Keyboard::Key keyUp, sf::Keyboard::Key keyDown,
					 sf::Keyboard::Key keyFire);

    private :
	    // ����������� ������� ���������� ����� �������
	    /////////////////////
	    sf::Keyboard::Key _keyLeft {sf::Keyboard::Key::Unknown};
		sf::Keyboard::Key _keyRight{sf::Keyboard::Key::Unknown};
		sf::Keyboard::Key _keyUp   {sf::Keyboard::Key::Unknown};
		sf::Keyboard::Key _keyDown {sf::Keyboard::Key::Unknown};
		sf::Keyboard::Key _keyFire {sf::Keyboard::Key::Unknown};
};