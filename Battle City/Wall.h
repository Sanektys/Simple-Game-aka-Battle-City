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
        /// ����� ��������� "������" ����� �� ���������
        /// </summary>
        /// <param name="rt">- ��������� �� ����, �� ������� ��������������
        /// ��� ��������� �������� ��� ���������</param>
        void mapRender(sf::RenderTexture* rt) override { rt->draw(_rectanglePinOnMap); }
        /// <summary>
        /// ��������� ����������� ��������� �� ���������, ������������� ������ �����
        /// </summary>
        void setMapPin() override;

        Wall(const Wall&) = delete;
        Wall operator=(const Wall&) = delete;

    protected :
        // �������� ������� ����� (1-3)
        int _variation{0};
};