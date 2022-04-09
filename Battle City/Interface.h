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
        /// ����� ��������� ���������� ����
        /// </summary>
        void render();

        Interface(const class Interface&) = delete;
        Interface operator=(const class Interface&) = delete;

    private :
        // ����� ���������� ����������
        std::unique_ptr<sf::Font> _debugFont;
        // ����� ��������� ������ � ��������� �������� ��������
        std::unique_ptr<sf::Font> _objectsDataFont;

        // ������ �� ������ ������� ������, �������� ����������� ���������
        class Game& _game;
};

