#pragma once

#include "Wall.h"


/// <summary>
/// ����� ������� �����
/// <para>���������� ������ ���������� ������������ ������ ���������</para>
/// </summary>
class SolidWall : public Wall
{
    public :
        SolidWall();
        ~SolidWall() override {}

        /// <summary>
        /// ����� ��������� �����
        /// </summary>
        /// <param name="rw">- ��������� �� ������ ���� ����</param>
        virtual void render(sf::RenderWindow* rw) override;
        /// <summary>
        /// ����� ������ �����������, �� ����� ��������� �����, �������
        /// </summary>
        /// <param name="rect">- ������������� "����������" ������
        /// ������ �� ������</param>
        virtual void setTextureRect(sf::IntRect rect) override;

    private :
        SolidWall(const SolidWall&) = delete;
        SolidWall operator=(const SolidWall&) = delete;
};