#pragma once

#include <chrono>
#include <array>

#include "Level.h"
#include "Interface.h"
#include "GameObject.h"


const size_t TERRAIN_OBJECTS_COUNT_MAX{1024};
const size_t ENTITY_OBJECTS_COUNT_MAX{128};

using std::chrono::steady_clock;


/// <summary>
/// ����� ������� ������
/// </summary>
class Game
{
    public :
        friend class Interface;

        Game() { _terrainCollision.fill(nullptr); }
        ~Game();

        /// <summary>
        /// ��������� ����� �������, ���� ����, �������� ������� � �������
        /// </summary>
        void setupSystem();
        /// <summary>
        /// �������� ������ �� ���������� ������� ��������.
        /// �������������� ������������ ��� ������������ �������.
        /// </summary>
        void initialize();
        /// <summary>
        /// �������� ���� ����, ������������ ������ ������ �������,
        /// ����������� ��� ���������� ������ ���� ��� ������ fps.
        /// <para>������������� �������� ���� ����,
        /// � ����������� �������� ������ ���������� � �������</para>
        /// </summary>
        /// <returns>���������� true ���� ���� ���� �� �������</returns>
        bool loop();
        /// <summary>
        /// ��������� ��� ������� �������, ���� ������� ����������
        /// </summary>
        void shutdown();

        /// <summary>
        /// �������� ������������ ������� ����������� �� ��� ����.
        /// ���� ������ ������ ���������� �� ������� �������,
        /// �� ������ �� �����
        /// </summary>
        /// <param name="type">��� ������������ �������</param>
        /// <param name="x">������� ���������� ���������� �� �����������</param>
        /// <param name="y">������� ���������� ���������� �� ���������</param>
        /// <returns>���������� ��������� �� ������� ����� ���������� �������,
        /// ���� ������ ��������� ���� ��� ������ ���������� � ������ ��� � ������� ����</returns>
        class std::unique_ptr<GameObject>& createObject(enum GameObjectType type,
                                                        float x, float y) const;
        /// <summary>
        /// ����������� ������� �������� ��� ����������
        /// � �������� ������������� �������
        /// </summary>
        /// <param name="object">��������� �� ������� ������,
        /// ������� ����� �������</param>
        void destroyObject(const class GameObject* object);

        /// <summary>
        /// �������� �� ����������� ������ ������� ������
        /// </summary>
        /// <param name="x">- ������� ��������� ������� �� �����������</param>
        /// <param name="y">- ������� ��������� ������� �� ���������</param>
        /// <param name="width">- ������ ��������� �������</param>
        /// <param name="height">- ������ ��������� �������</param>
        /// <param name="exceptObject">- ��������� �� ��� �������� ������</param>
        /// <param name="group">- �� ����� ������ ��������(���������/���������/����)
        /// ��������� �� �����������</param>
        /// <returns>���������� ��������� �� ������-������,
        /// ��� ������ ��������� ��� ���������� �����-���� ������</returns>
        class std::unique_ptr<GameObject>& checkIntersects(float x, float y,
            float width, float height, class GameObject* exceptObject,
            enum GameObjectGroup group = GameObjectGroup::ALL) const;
        /// <summary>
        /// ����������� ������� �� ������� x,y
        /// <para>���� ����������� ���������� (���� ��������),
        /// �� ������ ��������������� �� ���������� �����������</para>
        /// </summary>
        /// <param name="object">������������ ������</param>
        /// <param name="x">����� ���������� �� �����������</param>
        /// <param name="y">����� ���������� �� ���������</param>
        /// <returns>���������� false ���� ���� ������,
        /// � true ���� ����������� ������ �������</returns>
        bool moveObjectTo(class GameObject* object, float x, float y) const;

        /// <summary>
        /// ������� ���������� �������� ������������ ����
        /// </summary>
        /// <param name="type">��� �������������� ��������</param>
        /// <returns>���������� ���������� �������� ���� type</returns>
        int getObjectsCount(enum GameObjectType type) const;

        /// <summary>
        /// ��������� ������ ���������� ������������ �����������
        /// </summary>
        int getDiedEnemiesCount() const { return _diedEnemiesCount; }
        /// <summary>
        /// ���������� ���������� ������������ ����������� �� �������
        /// </summary>
        void increaseDiedEnemiesCount() const { ++_diedEnemiesCount; }

        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;

    private :
        /// <summary>
        /// ������� ����� ��������� ���� ����.
        /// �������� ����� �������������� ���������� ���������� � �.�.
        /// <para>��������� �������������� ������ ���������
        /// ��� ������� ������������� �������</para>
        /// ����� ������� ����� ������� �� ����������� � �������� �����
        /// </summary>
        void render();
        /// <summary>
        /// ������� ����� ���������� ������ ���� �� ������ ����
        /// <para>��������� ���������� ���������
        /// ���� ������������ �������� �� ������ �����</para>
        /// ��������� ����� ��� ������� � ��������� ������� "�����"
        /// </summary>
        /// <param name="dt">- �����, ����������� �� ���������� ������� ����,
        /// ��������� ��������� ������������ �������� ������� ��������,
        /// ��� ������� �� ������� ����������</param>
        void update(float dt);

        /// <summary>
        /// ����������� �������-��������� �� �������� ���������
        /// �� ����������� ��������� 1x1 �� �����
        /// <para>(��� ������� ����� ������� �������� �� ���������� ��������,
        /// ��� ��������� ���������� �������� ��� ��������)</para>
        /// </summary>
        /// <param name="owner">- ��������� �� ������ �������� ���������, �� ������� �������� ���� ������� ������� �����</param>
        /// <param name="order">- �������, � ������� ������� ������ �������� ��������� �� �����</param>
        void addCollisionCell(std::unique_ptr<GameObject>* owner, int order);

    private :
        // ������ ���������� �� ������� �������� ���������, ��������������� �� ��
        // ������������ �� ������� ����, �� �������� ������������ �������� ��������
        std::array<std::unique_ptr<GameObject>*, level::COLUMNS * level::ROWS>
            _terrainCollision;

        // ������ ���������� �� ��� ������� �������� ���������
        mutable std::array<std::unique_ptr<GameObject>, TERRAIN_OBJECTS_COUNT_MAX>
            _objectsTerrain;
        // ������ ���������� �� ��� ������� ������� ���������
        mutable std::array<std::unique_ptr<GameObject>, ENTITY_OBJECTS_COUNT_MAX>
            _objectsEntity;

        // ��������� ����
        std::unique_ptr<Interface> _interface;

        // ������� ����
        std::unique_ptr<sf::RenderWindow> _renderWindow;
        // ������ ������
        std::unique_ptr<sf::View> _playerCamera;

        // ��������� �� �������� ������� �������
        ///////////////////////
        std::unique_ptr<GameObject>* _base{nullptr};
        std::unique_ptr<GameObject>* _playerOne{nullptr};
        std::unique_ptr<GameObject>* _playerTwo{nullptr};

        // ����� ������ ����������� �����
        steady_clock::time_point _clockLastFrame;
        float _oneSecond{0.0f};
        // ���������� ������������� ������ � ������� �������
        int _updatesCount{0};
        // ���������� ������ � �������
        int _ups{0};

        mutable int _diedEnemiesCount{0};

        bool _isGameActive{true};
};