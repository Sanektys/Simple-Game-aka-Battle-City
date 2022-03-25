#pragma once

#include <chrono>

#include "Level.h"


const int OBJECTS_COUNT_MAX = 1024;

using std::chrono::steady_clock;

class GameObject;


/// <summary>
/// ����� ������� ������
/// </summary>
class Game
{
    public :
	    Game() {};
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
		/// ���� nullptr ���� ������ ��� ���������� � ������ ��� � ������� ����</returns>
		class GameObject* createObject(enum GameObjectType type, float x, float y);
		/// <summary>
		/// �������� ������������ ������� �� ������� �� �������
		/// </summary>
		/// <param name="i">������ �������� �������,
		/// ������� ����� �������</param>
		void destroyObject(int i) { 
			delete _objects[i]; 
			_objects[i] = nullptr;
		}
		/// <summary>
		/// ����������� ������� �������� ��� ����������
		/// � �������� ������������� �������
		/// </summary>
		/// <param name="object">��������� �� ������� ������,
		/// ������� ����� �������</param>
		void destroyObject(const GameObject* object);

		/// <summary>
		/// �������� �� ����������� ������ ������� ������
		/// </summary>
		/// <param name="x">������� ��������� ������� �� �����������</param>
		/// <param name="y">������� ��������� ������� �� ���������</param>
		/// <param name="width">������ ��������� �������</param>
		/// <param name="height">������ ��������� �������</param>
		/// <param name="exceptObject">��������� �� ��� �������� ������</param>
		/// <returns>���������� ��������� �� ������-������,
		/// ��� nullptr ��� ���������� �����-���� ������</returns>
		class GameObject* checkIntersects(float x, float y, float width, float height,
			                              class GameObject* exceptObject) const;
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
		void increaseDiedEnemiesCount() { ++_diedEnemiesCount; }

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

		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

    private :
	    // ������ ���������� �� ��� ������� �������
		class GameObject* _objects[OBJECTS_COUNT_MAX]{};
		class GameObject* _base{nullptr};
		class GameObject* _playerOne{nullptr};
		class GameObject* _playerTwo{nullptr};

		// ������� ����
		sf::RenderWindow* _renderWindow{nullptr};
		// ������ ������
		sf::View* _playerCamera{nullptr};
		// ����� ���������� ����������
		sf::Font* _debugFont{nullptr};

		// ����� ������ ����������� �����
		steady_clock::time_point _clockLastFrame;
		float _oneSecond{0.0f};
		int _diedEnemiesCount{0};
		// ���������� ������������� ������ � ������� �������
		int _updatesCount{0};
		// ���������� ������ � �������
		int _ups{0};

		bool _isGameActive{true};
};