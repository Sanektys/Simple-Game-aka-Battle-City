#pragma once

#include "Level.h"


class Game;


/// <summary>
/// �����, ���������� �� ��������� ����
/// </summary>
class Interface
{
    public :
        Interface() = delete;
        explicit Interface(class Game&);

        /// <summary>
        /// ����� ��������� ����������� �������� � ��������� ���������� ����
        /// </summary>
        void renderStats();
        /// <summary>
        /// ����� ��������� ���������
        /// </summary>
        void renderMap();
        /// <summary>
        /// ����� ������� ������� �� ���������� ���������� ���������
        /// </summary>
        /// <param name="dt">- ������ �������,
        /// ������������ �� ������� ������� ����</param>
        void mapUpdate(float dt) { _mapUpdateTimer += dt; }

        Interface(const class Interface&) = delete;
        Interface operator=(const class Interface&) = delete;

    private :
        // ����� ���������� ����������
        std::unique_ptr<sf::Font> _debugFont;
        // ����� ��������� ������ � ��������� �������� ��������
        std::unique_ptr<sf::Font> _objectsDataFont;

        // ����, �� ������� ������������� ���������, ������������ ������� �������,
        // � ����� ���������� ��������� �� ���������
        std::unique_ptr<sf::RenderTexture> _mapLayout;
        sf::Sprite _map;  // ���������
        sf::RectangleShape _mapFrame;  // �������� ��� ���������

        float _mapUpdateTimer{2.0f};  // ������ �� ���������� ���������� ���������
        const float MAP_UPDATE_PERIOD{2.0f};  // ����� ����� ���������� ������� ����������� ���������

        // ������ �� ������ ������� ������, �������� ����������� ���������
        class Game& _game;
};

