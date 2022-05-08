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
        ~Interface() = default;

        /// <summary>
        /// ����� ��������� ����������� �������� � ��������� ���������� ����
        /// </summary>
        void renderStats();
        /// <summary>
        /// ����� ��������� ���������
        /// </summary>
        void renderMap();
        /// <summary>
        /// �����, ����������� ������� ����������,
        /// ������� �������� �� ������������ ��������
        /// ��������� � ��������� �������� ����������
        /// </summary>
        /// <param name="dt">- ������ �������,
        /// ������������ �� ���������� ������� ����</param>
        void timersUpdate(float dt);
        /// <summary>
        /// ���������� ���������� �� �������� ������� �������,
        /// ������� ���������� �� ������ ������
        /// </summary>
        void updateGameObjectsPointers();

        Interface(const class Interface&) = delete;
        Interface operator=(const class Interface&) = delete;

    private :
        /// <summary>
        /// ����� ��������� ������ �������� ����
        /// </summary>
        void showBaseHealth();
        /// <summary>
        /// ����� ��������� ������ �������� ����� ������
        /// </summary>
        void showTankHealth();
        /// <summary>
        /// ����� ��������� ����������(������) ����������� ����� ������
        /// </summary>
        void showTankRecharge();

    private :
        // ����� ���������� ����������
        sf::Font _debugFont;
        // ����� ��������� ������ � ��������� �������� ��������
        sf::Font _objectsDataFont;

        // ����, �� ������� ������������� ���������, ������������ ������� �������,
        // � ����� ���������� ��������� �� ���������
        std::unique_ptr<sf::RenderTexture> _mapLayout;
        sf::Sprite _map;  // ���������
        sf::RectangleShape _mapFrame;  // �������� ��� ���������

        float _mapUpdateTimer{2.0f};  // ������ �� ���������� ���������� ���������
        const float MAP_UPDATE_PERIOD{2.0f};  // ����� ����� ���������� ������� ����������� ���������

        // ���������� �������� ���������� ��� ��������� ����� ��������� ����/������
        const int NUMBER_OF_BLINKS{3};
        const float BLINK_TIME{0.1f};  // ����� ������ ��������

        ///////////////////////////////////////////////////
        // ������ ����� ��������� ����
        sf::RectangleShape _baseHealthFrame;  // �������� ������ �������� ����
        sf::RectangleShape _baseHealthBar;    // ������ �������� ����
        sf::Text _baseHealthText;             // ������� ������ �������� ����
        sf::Color _baseHealthColor;           // ���� ������� � ������ ������ �������� ����
        float _deltaBaseHealthBar{0.0f};      // ����� ����� ������, ������ ������ ���� ��������� ����
        unsigned _deltaBaseHealthColorShift{0u};  // ������� ����� ������� � ������ ��� ��������� �� ���� ���� ��������� ����
        int _baseHealthState{0};              // ���������� ����� ��������� ����, ���������� �� ������� ����������
        float _baseHealthBlinkTimer{0.0f};    // ������, ���������� �� �������� ��� ��������� ����� ��������� ����

        ///////////////////////////////////////////////////
        // ������ ����� ��������� ����� ������
        sf::RectangleShape _tankHealthFrame;  // �������� ������ �������� ����� ������
        sf::RectangleShape _tankHealthBar;    // ������ �������� ����� ������
        sf::Text _tankHealthText;             // ������� ������ �������� ����� ������
        sf::Color _tankHealthColor;           // ���� ������� � ������ ������ �������� ����� ������
        float _deltaTankHealthBar{0.0f};      // ����� ����� ������, ������ ������ ���� ��������� ����� ������
        unsigned _deltaTankHealthColorShift{0u};  // ������� ����� ������� � ������ ��� ��������� �� ���� ���� ��������� �����
        int _tankHealthState{0};              // ���������� ����� ��������� ����� ������, ���������� �� ������� ����������
        float _tankHealthBlinkTimer{0.0f};    // ������, ���������� �� �������� ��� ��������� ����� ��������� ����� ������

        ///////////////////////////////////////////////////
        // ������ ���������� ����������� ����� ������
        sf::RectangleShape _rechargeFrame;  // �������� ���������� ����������� ����� ������
        sf::RectangleShape _rechargeBar;    // ������ ���������� ����������� ����� ������
        float _deltaRechargeBar{0.0f};      // ����� ����� ���������� �����������, ������ ������� ������� ���������� ����������
        const float RECHARGE_BAR_UPDATE_PERIOD{0.25f};  // ����� ���������� ���������� �����������
        float _rechargeBarUpdateTimer{0.0f};  // ������, ������������� ����� �� ���������� ���������� ���������� �����������

        // ������ �� ������ ������� ������, �������� ����������� ���������
        class Game& _game;

        class TankPlayer* _tankPlayer{nullptr};  // ��������� �� ������ ����� ������
        class Base* _base{nullptr};  // ��������� �� ������ ����
};

