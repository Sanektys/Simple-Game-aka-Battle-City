#pragma once

#include <ctime>

#include "Level.h"


const int OBJECTS_COUNT_MAX = 1024;


class GameObject;


/// <summary>
/// Класс игровой логики
/// </summary>
class Game
{
    public :
	    Game() {};
		~Game();

		void setupSystem();
		void initialize();
		bool loop();
		void shutdown();

		class GameObject* createObject(enum GameObjectType type, float x, float y);
		void destroyObject(int i) { 
			delete _objects[i]; 
			_objects[i] = nullptr;
		}
		void destroyObject(const GameObject* object);

		// Проверка пересечения одного объекта другим
		class GameObject* checkIntersects(float x, float y, float width, float height, 
			                              class GameObject* exceptObject);
		// Проверка на возможность объекта переместиться на позицию x,y
		bool moveObjectTo(class GameObject* object, float x, float y);
		// Пересчёт всех объектов определённого типа
		int getObjectsCount(enum GameObjectType type);

		int getDiedEnemiesCount() { return _diedEnemiesCount; }
		void increaseDiedEnemiesCount() { ++_diedEnemiesCount; }

    private :
		void render();
		void update(float dt);

		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

    private :
		class GameObject* _objects[OBJECTS_COUNT_MAX]{};
		class GameObject* _base{nullptr};
		class GameObject* _playerOne{nullptr};
		class GameObject* _playerTwo{nullptr};

		sf::RenderWindow* _renderWindow{nullptr};
		sf::Font* _debugFont{nullptr};

		clock_t _clockLastFrame{0};

		int _diedEnemiesCount{0};
		float _oneSecond{0.0f};
		int _updatesCount{0};
		int _ups{0};

		bool _isGameActive{true};
};