#pragma once

#include <chrono>

#include "Level.h"


const int OBJECTS_COUNT_MAX = 1024;

using std::chrono::steady_clock;

class GameObject;


/// <summary>
/// Класс игровой логики
/// </summary>
class Game
{
    public :
	    Game() {};
		~Game();

		/// <summary>
		/// Установка зерна рандома, окна игры, загрузка атласов и шрифтов
		/// </summary>
		void setupSystem();
		/// <summary>
		/// Загрузка уровня из двумерного массива символов.
		/// Предварительно уничтожаются все существующие объекты.
		/// </summary>
		void initialize();
		/// <summary>
		/// Основной цикл игры, производится расчёт дельты времени,
		/// необходимой для корректной работы игры при разном fps.
		/// <para>Отслеживается закрытие окна игры,
		/// и запускаются основные методы обновления и рендера</para>
		/// </summary>
		/// <returns>Возвращает true если окно игры не закрыто</returns>
		bool loop();
		/// <summary>
		/// Удаляются все игровые объекты, если таковые существуют
		/// </summary>
		void shutdown();

		/// <summary>
		/// Создание определённого объекта основываясь на его типе.
		/// Если объект нельзя разместить на целевой позиции,
		/// он создан не будет
		/// </summary>
		/// <param name="type">тип создаваемого объекта</param>
		/// <param name="x">целевая координата размещения по горизонтали</param>
		/// <param name="y">целевая координата размещения по вертикали</param>
		/// <returns>Возвращает указатель на базовый класс созданного объекта,
		/// либо nullptr если нельзя его разместить в памяти или в игровом поле</returns>
		class GameObject* createObject(enum GameObjectType type, float x, float y);
		/// <summary>
		/// Удаление определённого объекта из массива по индексу
		/// </summary>
		/// <param name="i">индекс целевого объекта,
		/// который нужно удалить</param>
		void destroyObject(int i) { 
			delete _objects[i]; 
			_objects[i] = nullptr;
		}
		/// <summary>
		/// Прохождение массива объектов для нахождения
		/// и удаления определенного объекта
		/// </summary>
		/// <param name="object">указатель на целевой объект,
		/// который нужно удалить</param>
		void destroyObject(const GameObject* object);

		/// <summary>
		/// Проверка на пересечение одного объекта другим
		/// </summary>
		/// <param name="x">позиция исходного объекта по горизонтали</param>
		/// <param name="y">позиция исходного объекта по вертикали</param>
		/// <param name="width">ширина исходного объекта</param>
		/// <param name="height">высота исходного объекта</param>
		/// <param name="exceptObject">указатель на сам исходный объект</param>
		/// <returns>Возвращает указатель на объект-помеху,
		/// или nullptr при отсутствии какой-либо помехи</returns>
		class GameObject* checkIntersects(float x, float y, float width, float height,
			                              class GameObject* exceptObject) const;
		/// <summary>
		/// Перемещение объекта на позицию x,y
		/// <para>Если перемещение невозможно (есть преграда),
		/// то объект останавливается на предыдущих координатах</para>
		/// </summary>
		/// <param name="object">перемещаемый объект</param>
		/// <param name="x">новая координата по горизонтали</param>
		/// <param name="y">новая координата по вертикали</param>
		/// <returns>Возвращает false если есть помеха,
		/// и true если перемещение прошло успешно</returns>
		bool moveObjectTo(class GameObject* object, float x, float y) const;
		/// <summary>
		/// Подсчёт количества объектов определённого типа
		/// </summary>
		/// <param name="type">тип подсчитываемых объектов</param>
		/// <returns>Возвращает количество объектов типа type</returns>
		int getObjectsCount(enum GameObjectType type) const;

		/// <summary>
		/// Получение общего количества уничтоженных противников
		/// </summary>
		int getDiedEnemiesCount() const { return _diedEnemiesCount; }
		/// <summary>
		/// Увеличение количества уничтоженных противников на единицу
		/// </summary>
		void increaseDiedEnemiesCount() { ++_diedEnemiesCount; }

    private :
		/// <summary>
		/// Главный метод отрисовки окна игры.
		/// Включает вывод дополнительной отладочной информации и т.д.
		/// <para>Запускает индивидуальные методы отрисовки
		/// для каждого существующего объекта</para>
		/// Перед началом кадра очищает всё отображение с прошлого кадра
		/// </summary>
		void render();
		/// <summary>
		/// Главный метод обновления логики игры на каждый такт
		/// <para>Запускает обновление состояния
		/// всех существующих объектов на каждом такте</para>
		/// Проверяет успех или неуспех в выполении игровых "задач"
		/// </summary>
		/// <param name="dt">- время, затраченное на предыдущий игровой такт,
		/// позволяет корректно рассчитывать действия игровых объектов,
		/// что зависят от времени выполнения</param>
		void update(float dt);

		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

    private :
	    // Массив указателей на все игровые объекты
		class GameObject* _objects[OBJECTS_COUNT_MAX]{};
		class GameObject* _base{nullptr};
		class GameObject* _playerOne{nullptr};
		class GameObject* _playerTwo{nullptr};

		// Игровое окно
		sf::RenderWindow* _renderWindow{nullptr};
		// Камера игрока
		sf::View* _playerCamera{nullptr};
		// Шрифт отладочной информации
		sf::Font* _debugFont{nullptr};

		// Время начала предыдущего кадра
		steady_clock::time_point _clockLastFrame;
		float _oneSecond{0.0f};
		int _diedEnemiesCount{0};
		// Количество промежуточных тактов в течение секунды
		int _updatesCount{0};
		// Количество тактов в секунду
		int _ups{0};

		bool _isGameActive{true};
};