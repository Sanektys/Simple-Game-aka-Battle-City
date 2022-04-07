#pragma once

#include "Wall.h"


/// <summary>
/// ����� ������� �����
/// <para>���������� ������ ���������� ������������ ������ ���������</para>
/// </summary>
class SolidWall : public Wall
{
    public :
        SolidWall(const class Game& game, sf::IntRect rect);
        ~SolidWall() override {}

        /// <summary>
        /// ����� ��������� �����
        /// </summary>
        /// <param name="rw">- ��������� �� ������ ���� ����</param>
        virtual void render(sf::RenderWindow* rw) override;

        SolidWall(const SolidWall&) = delete;
        SolidWall operator=(const SolidWall&) = delete;

    private :
        /// <summary>
        /// ����� ������ �����������, �� ����� ��������� �����, �������
        /// </summary>
        /// <param name="rect">- ������������� "����������" ������
        /// ������ �� ������</param>
        virtual void setTextureRect(sf::IntRect rect) override;
};