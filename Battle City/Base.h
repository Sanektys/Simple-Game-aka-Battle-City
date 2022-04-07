#pragma once

#include "GameObject.h"


/// <summary>
/// ����� ������� ����
/// </summary>
class Base : public GameObject
{
    public :
        Base(const class Game&, sf::IntRect);
        ~Base() override {}

        /// <summary>
        /// ����� ��������� ������� ������� ����
        /// </summary>
        /// <param name="rw">- ��������� �� ������ �������� ����</param>
        virtual void render(sf::RenderWindow* rw) override;
        /// <summary>
        /// ����� ���������� ��������� ������� ������� ����
        /// </summary>
        /// <param name="dt">- ������ �������,
        /// ������������ �� ���������� ������� ����</param>
        virtual void update(float dt) override;

        Base(const Base&) = delete;
        Base operator=(const Base&) = delete;

    private :
        /// <summary>
        /// ���������� ������� ���� � ����������� �� ���������� ����� ���������
        /// <para>����� ����� ������������ �������� ���� ��� �������
        /// ������������ ������� ����</para>
        /// </summary>
        /// <param name="rect">- ������������� "����������" ����������� ������
        /// �� ����� ������</param>
        virtual void setTextureRect(sf::IntRect rect) override;

    private :
        // ���������� ��� ����� ������� ����
        ////////////////	
        float _frameSwitchTime{0.0f};
        short _horizontalFrameNumber{1};
        bool _toLeft{true};
};