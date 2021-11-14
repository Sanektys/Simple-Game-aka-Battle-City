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
		Game();
		~Game();

		void setupSystem();
		void initialize();
		bool loop();
		void shutdown();

		class GameObject* createObject(enum GameObjectType type, float x, float y);
		void destroyObject(class GameObject* object);

		// Проверка пересечения одного объекта другим
		class GameObject* checkIntersects(float x, float y, float width, float height, 
			                              class GameObject* exceptObject);
		// Проверка на возможность объекта переместиться на позицию x,y
		bool moveObjectTo(class GameObject* object, float x, float y);
		// Пересчёт всех объектов определённого типа
		int getObjectsCount(enum GameObjectType type);

		int getDiedEnemiesCount() { return _diedEnemiesCount; }
		void increaseDiedEnemiesCount();

    private :
		void render();
		void update(float dt);

    private :
		bool _isGameActive;
		clock_t _clockLastFrame;

		sf::RenderWindow* _renderWindow;

		sf::Font* _debugFont;

		class GameObject* _objects[OBJECTS_COUNT_MAX];

		class GameObject* _base;
		class GameObject* _playerOne;
		class GameObject* _playerTwo;

		int _diedEnemiesCount;
		float _oneSecond;
		int _updatesCount;
		int _ups;
};