#pragma once

#include "Tank.h"


/// <summary>
/// ����� ������������ ������� �����
/// </summary>
class TankPlayer : public Tank
{
    public :
        TankPlayer(const class Game& game, sf::IntRect rect,
                   sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight,
                   sf::Keyboard::Key keyUp,   sf::Keyboard::Key keyDown,
                   sf::Keyboard::Key keyFire);
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
        /// ����� ���������� �������� ������ ��� ���������� ������
        /// </summary>
        /// <param name="keyLeft">- ������� ��� �������� ������</param>
        /// <param name="keyRight">- ������� ��� �������� �������</param>
        /// <param name="keyUp">- ������� ��� �������� �����</param>
        /// <param name="keyDown">- ������� ��� �������� ����</param>
        /// <param name="keyFire">- ������� ��� ��������</param>
        void setKeys(sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight,
                     sf::Keyboard::Key keyUp,   sf::Keyboard::Key keyDown,
                     sf::Keyboard::Key keyFire);
        /// <summary>
        /// ����� ���������� �������������� ������ ��� ���������� ������
        /// </summary>
        /// <param name="keyLeft">- ������� ��� �������� ������</param>
        /// <param name="keyRight">- ������� ��� �������� �������</param>
        /// <param name="keyUp">- ������� ��� �������� �����</param>
        /// <param name="keyDown">- ������� ��� �������� ����</param>
        /// <param name="keyFire">- ������� ��� ��������</param>
        void setAltKeys(sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight,
                        sf::Keyboard::Key keyUp,   sf::Keyboard::Key keyDown,
                        sf::Keyboard::Key keyFire);

        TankPlayer(const TankPlayer&) = delete;
        TankPlayer operator=(const TankPlayer&) = delete;

    private :
        // ����������� ������� ���������� ����� �������
        /////////////////////
        sf::Keyboard::Key _keyLeft {sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _keyRight{sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _keyUp   {sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _keyDown {sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _keyFire {sf::Keyboard::Key::Unknown};
        /////////////////////
        sf::Keyboard::Key _altKeyLeft {sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _altKeyRight{sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _altKeyUp   {sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _altKeyDown {sf::Keyboard::Key::Unknown};
        sf::Keyboard::Key _altKeyFire {sf::Keyboard::Key::Unknown};
};