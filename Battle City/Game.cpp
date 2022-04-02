#include <cstring>
#include <random>

#include "Game.h"
#include "Wall.h"
#include "SolidWall.h"
#include "Base.h"
#include "Bullet.h"
#include "TankPlayer.h"
#include "TankEnemy.h"
#include "EnemySpawner.h"


///////////////////////////////////////////////////////////
// Глобальные переменные
namespace level {
    sf::Texture* ATLAS_TERRAIN;
    sf::Texture* ATLAS_ENTITY;
}
std::mt19937* RANDOM;


Game::~Game() {
    if (level::ATLAS_TERRAIN)
        delete level::ATLAS_TERRAIN;
    if (level::ATLAS_ENTITY)
        delete level::ATLAS_ENTITY;
    if (RANDOM)
        delete RANDOM;
}

void Game::setupSystem() {
	// Установка зерна рандома
    RANDOM = new std::mt19937;
	RANDOM->seed(unsigned(steady_clock::now().time_since_epoch().count()));

	// Создание игрового окна
	_renderWindow.reset(new sf::RenderWindow(sf::VideoMode::getDesktopMode(),
                                             "Battle City", sf::Style::Fullscreen));
	_renderWindow->setMouseCursorVisible(false);

	// Иницализация камеры игрока
	_playerCamera.reset(new sf::View(
		sf::FloatRect(0.0f, 0.0f,
					  level::CAMERA_WIDTH, level::CAMERA_HEIGHT)));

	// Загрузка текстур из атласов
	level::ATLAS_TERRAIN = new sf::Texture();
	level::ATLAS_TERRAIN->loadFromFile("atlas_terrain.png");
	level::ATLAS_ENTITY = new sf::Texture();
	level::ATLAS_ENTITY->loadFromFile("atlas_entity.png");

	// Загрузка шрифтов
	_debugFont.reset(new sf::Font);
	_debugFont->loadFromFile("progresspixel-bold.ttf");
}

void Game::initialize() {
	shutdown();

	_diedEnemiesCount = 0;

	// Загрузка уровня
	for (int r = 0; r < level::ROWS; r++) {
		for (int c = 0; c < level::COLUMNS; c++) {
			unsigned char cellSymbol = level::FIRST_MAP[r][c];

			switch (cellSymbol) {
                case level::SYMBOL_BRICK_WALL :
				    createObject(GameObjectType::WALL, (float)c, (float)r);
					break;

                case level::SYMBOL_SOLID_WALL :
					createObject(GameObjectType::SOLID_WALL, (float)c, (float)r);
					break;
                    
                case level::SYMBOL_STEEL_WALL :
				    createObject(GameObjectType::STEEL_WALL, (float)c, (float)r);
				    break;
			    
                case level::SYMBOL_BASE :
					_base = &createObject(GameObjectType::BASE, (float)c, (float)r);
					break;
				
                case level::SYMBOL_PLAYER_1 :
					_playerOne = &createObject(
						GameObjectType::TANK_FIRST_PLAYER, (float)c, (float)r);
					break;
				
                case level::SYMBOL_PLAYER_2 :
                    _playerTwo = &createObject(
                        GameObjectType::TANK_SECOND_PLAYER, (float)c, (float)r);
					break;
				
                case level::SYMBOL_ENEMY_SPAWNER :
					//createObject(GameObjectType::ENEMY_SPAWNER, (float)c, (float)r);
					break;
			}
		}
	}
}

bool Game::loop() {
    // Обработка закрытия игрового окна
	if (!_renderWindow->isOpen())
		return false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		_renderWindow->close();
		return false;
	}

    using std::chrono::duration;
    using std::chrono::duration_cast;
    steady_clock::time_point clockNow;

    // Постановка игры на паузу
    bool wasPause{false};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Pause)) {
        wasPause = true;
        clockNow = steady_clock::now();
        while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) &&
               !sf::Keyboard::isKeyPressed(sf::Keyboard::Space));        
    }

	// Расчёт прошедшего времени за такт
    if (!wasPause)
        clockNow = steady_clock::now();
	float deltaTime =
		duration_cast<duration<float>>(clockNow - _clockLastFrame).count();
    if (!wasPause)
        _clockLastFrame = clockNow;
    else
        _clockLastFrame = steady_clock::now();

	// Счётчик обновлений в секунду (тактов в секунду)
	_oneSecond += deltaTime;
	_updatesCount++;
	if (_oneSecond >= 1.0f) {
		_oneSecond = 0.0f;
		_ups = _updatesCount;
		_updatesCount = 0;
	}

	// Отслеживание закрытия окна
	sf::Event event;
	while (_renderWindow->pollEvent(event))
		if (event.type == sf::Event::Closed)
			_renderWindow->close();

	update(deltaTime);
	render();

	return _isGameActive;
}

void Game::shutdown() {
    for (auto& pointer : _objectsTerrain)
        if (pointer)
            pointer.reset();

    for (auto& pointer : _objectsEntity)
        if (pointer)
            pointer.reset();
}

void Game::render() {
    std::string string;
    sf::Text text;
    sf::RectangleShape rectangle;
    text.setFont(*_debugFont);
    text.setCharacterSize(24);
    text.setLetterSpacing(1.6f);

    // Начало кадра
    _renderWindow->clear(sf::Color(20, 20, 20));

    // Обновление положения камеры вслед за танком игрока
    // Смена вида делается перед отрисовкой игровых объектов,
    // чтобы они были увеличены и корректно отображались относительно танка
    _playerCamera->setCenter(((*_playerTwo)->getX()
                             + (*_playerTwo)->getWidth() / 2.0f) * level::PIXELS_PER_CELL,
                             ((*_playerTwo)->getY()
                             + (*_playerTwo)->getHeight() / 2.0f) * level::PIXELS_PER_CELL);
    _renderWindow->setView(*_playerCamera);

    int objectsCount = 0;
    // Отрисовка всех игровых объектов
    for (auto& pointer : _objectsTerrain)
        if (pointer) {
            pointer->render(&*_renderWindow);
            objectsCount++;
        }
    for (auto& pointer : _objectsEntity)
        if (pointer) {
            pointer->render(&*_renderWindow);
            objectsCount++;
        }
	
	// Сброс вида до разрешения монитора
	// Всё, что идёт дальше, отрисовывается относительно главного экрана
	_renderWindow->setView(_renderWindow->getDefaultView());

	// Счётчик обновлений в секунду
	string = "UPS: " + std::to_string(_ups);
	text.setString(string);
	text.setPosition(_renderWindow->getSize().x - text.getGlobalBounds().width, 2.0f);
	text.setFillColor(sf::Color::Color(165, 92, 126, 255));
	rectangle.setSize(sf::Vector2f(text.getGlobalBounds().width + 8.0f,
					               text.getGlobalBounds().height + 8.0f));
	rectangle.setPosition(sf::Vector2f(text.getGlobalBounds().left - 4.0f,
						               text.getGlobalBounds().top - 4.0f));
	rectangle.setFillColor(sf::Color::Color(20, 20, 20, 220));
	_renderWindow->draw(rectangle);
	_renderWindow->draw(text);
	
	// Счётчик объектов в игре
    #ifdef _DEBUG
	string = "Objects: " + std::to_string(objectsCount);
	text.setString(string);
	text.setPosition(_renderWindow->getSize().x - text.getGlobalBounds().width, 34.0f);
	text.setFillColor(sf::Color::Color(125, 155, 185, 200));
	rectangle.setSize(sf::Vector2f(text.getGlobalBounds().width + 8.0f,
					               text.getGlobalBounds().height + 8.0f));
	rectangle.setPosition(sf::Vector2f(text.getGlobalBounds().left - 4.0f,
						               text.getGlobalBounds().top - 4.0f));
	rectangle.setFillColor(sf::Color::Color(20, 20, 20, 220));
	_renderWindow->draw(rectangle);
	_renderWindow->draw(text);

	// Текущее разрешение
	string = std::to_string(sf::VideoMode::getDesktopMode().width) + "x"
		     + std::to_string(sf::VideoMode::getDesktopMode().height);
	text.setString(string);
	text.setPosition(0.0f, 0.0f);
	text.setFillColor(sf::Color::Color(125, 155, 185, 200));
	rectangle.setSize(sf::Vector2f(text.getGlobalBounds().width + 8.0f,
					               text.getGlobalBounds().height + 8.0f));
	rectangle.setPosition(sf::Vector2f(text.getGlobalBounds().left - 4.0f,
						               text.getGlobalBounds().top - 4.0f));
	rectangle.setFillColor(sf::Color::Color(20, 20, 20, 220));
	_renderWindow->draw(rectangle);
	_renderWindow->draw(text);
    #endif

	// Конец кадра
	_renderWindow->display();
}

void Game::update(float dt) {
    // Обновление логики всех существующих объектов
    for (auto& pointer : _objectsTerrain)
        if (pointer) {
            pointer->update(dt);

            if (pointer->getHealth() <= 0
                && pointer->getDestroyAfterDeath())
                pointer.reset();
        }
    for (auto& pointer : _objectsEntity)
        if (pointer) {
            pointer->update(dt);

            if (pointer->getHealth() <= 0
                && pointer->getDestroyAfterDeath())
                pointer.reset();
        }

    // При уничтожении базы
    if (*_base && (*_base)->getHealth() <= 0)
        initialize();

	// Уничтожение игроков
	if (*_playerOne && (*_playerOne)->getHealth() <= 0)
		_playerOne->reset();

	if (*_playerTwo && (*_playerTwo)->getHealth() <= 0)
		initialize();

	// Все противники уничтожены
	if (_diedEnemiesCount == level::tank::enemy::PER_LEVEL)
		initialize();
}

std::unique_ptr<GameObject>& Game::checkIntersects(float x, float y,
                                                   float width, float height,
	                                               class GameObject* exceptObject) const {
    static std::unique_ptr<GameObject> returnObject;

	// Левый верхний угол входного объекта
	float primaryCoordY = y;
	float primaryCoordX = x;
	// Правый нижний угол входного объекта
	float overallCoordY = primaryCoordY + height - 0.00001f;
	float overallCoordX = primaryCoordX + width - 0.00001f;
	
	for (auto& pointer : _objectsTerrain)
		if (pointer && &*pointer != exceptObject && pointer->getPhysical()) {
			float pcY = pointer->getY();
			float pcX = pointer->getX();
			float ocY = pcY + pointer->getHeight() - 0.00001f;
			float ocX = pcX + pointer->getWidth() - 0.00001f;
			
			bool conditionOne   = primaryCoordY <= ocY;
			bool conditionTwo   = overallCoordY >= pcY;
			bool conditionThree = primaryCoordX <= ocX;
			bool conditionFour  = overallCoordX >= pcX;
			
			if (conditionOne && conditionTwo && conditionThree && conditionFour)
				return pointer;    // При пересечении вернуть указатель объект-помеху
		}
    for (auto& pointer : _objectsEntity)
        if (pointer && &*pointer != exceptObject && pointer->getPhysical()) {
            float pcY = pointer->getY();
            float pcX = pointer->getX();
            float ocY = pcY + pointer->getHeight() - 0.00001f;
            float ocX = pcX + pointer->getWidth() - 0.00001f;

            bool conditionOne   = primaryCoordY <= ocY;
            bool conditionTwo   = overallCoordY >= pcY;
            bool conditionThree = primaryCoordX <= ocX;
            bool conditionFour  = overallCoordX >= pcX;

            if (conditionOne && conditionTwo && conditionThree && conditionFour)
                return pointer;    // При пересечении вернуть указатель объект-помеху
        }

	return returnObject;
}

bool Game::moveObjectTo(class GameObject* object, float x, float y) const {
	// Координаты желаемой(новой) позиции
	float newCoordY = y;
	float newCoordX = x;
	// Габариты объекта на новой позиции
	float newOverallCoordY = newCoordY + object->getHeight() - 0.00001f;
	float newOverallCoordX = newCoordX + object->getWidth() - 0.00001f;

	// Проверка на неверные координаты (выход за пределы игрового поля)
	bool conditionOne   = newCoordY < 0.0f;
	bool conditionTwo   = newCoordX < 0.0f;
	bool conditionThree = newOverallCoordY >= level::ROWS;
	bool conditionFour  = newOverallCoordX >= level::COLUMNS;
	
	if (conditionOne || conditionTwo || conditionThree || conditionFour)
		return false;

	// Проверка на пересечение с другим объектом будто
	// искомый объект уже на новой позиции
    std::unique_ptr<GameObject>& otherObject{checkIntersects(x, y, object->getWidth(),
                                                             object->getHeight(), object)};

	// Если указатель ненулевой(есть пересечение с другим объектом),
	// то выполнить поведение при столкновении
	if (otherObject) {
		object->intersect(&*otherObject);
		object->setYSpeed(0.0f);
		object->setXSpeed(0.0f);
		return false;
	} else {    //Иначе поставить объект на новую позицию
		object->setX(x);
		object->setY(y);
		return true;
	}
}

int Game::getObjectsCount(enum GameObjectType type) const {
	int totalCount = 0;
    
    if (int(type) >= 10 && int(type) < 100) {
        for (auto& pointer : _objectsTerrain)
            if (pointer && pointer->getType() == type)
                totalCount++;
    }
    else if (int(type) >= 100 && int(type) < 1000) {
        for (auto& pointer : _objectsEntity)
            if (pointer && pointer->getType() == type)
                totalCount++;
    }

	return totalCount;
}

std::unique_ptr<GameObject>& Game::createObject(enum GameObjectType type,
                                                float x, float y) const {
    static std::unique_ptr<GameObject> object;

	switch (type) {
        case GameObjectType::WALL :
            object.reset(new Wall(*this, level::wall::BRICK_IMAGE));
            break;

        case GameObjectType::STEEL_WALL :
            object.reset(new Wall(*this, level::wall::STEEL_IMAGE, true));
            break;

        case GameObjectType::SOLID_WALL :
            object.reset(new SolidWall(*this, level::wall::SOLID_BRICK_IMAGE));
            break;
				
        case GameObjectType::BASE :
            object.reset(new Base(*this, level::base::IMAGE));
            break;
				
        case GameObjectType::TANK_FIRST_PLAYER :
            object.reset(new TankPlayer(*this, level::tank::player::FIRST_IMAGE,
                                        sf::Keyboard::Left, sf::Keyboard::Right,
                                        sf::Keyboard::Up,   sf::Keyboard::Down,
                                        sf::Keyboard::Space));
            break;

        case GameObjectType::TANK_SECOND_PLAYER :
            object.reset(new TankPlayer(*this, level::tank::player::SECOND_IMAGE,
                                        sf::Keyboard::A, sf::Keyboard::D,
                                        sf::Keyboard::W, sf::Keyboard::S,
                                        sf::Keyboard::E));
            break;
				
        case GameObjectType::TANK_ENEMY :
            object.reset(new TankEnemy(*this, level::tank::enemy::basic::IMAGE));
            break;
				
        case GameObjectType::BULLET :
            object.reset(new Bullet(*this, level::bullet::basic::IMAGE));
            break;
				
        case GameObjectType::ENEMY_SPAWNER :
            object.reset(new EnemySpawner(*this));
            break;
    }

    if (!object)
        return object;

    if (moveObjectTo(&*object, x, y) == false) {
        object.reset();
        return object;
    }

    // Нахождение свободного указателя и присвоение нового экземпляра игровой сущности
    switch (object->getGroup()) {
        case GameObjectGroup::TERRAIN :
            for (auto& objectTerrain : _objectsTerrain) {
                if (!objectTerrain) {
                    objectTerrain.reset(object.release());
                    return objectTerrain;
                }
            }
            object.reset();
            break;

        case GameObjectGroup::ENTITY :
            for (auto& objectEntity : _objectsEntity) {
                if (!objectEntity) {
                    objectEntity.reset(object.release());
                    return objectEntity;
                }
            }
            object.reset();
            break;
    }
    return object;
}

void Game::destroyObject(const class GameObject* object) {
    switch (object->getGroup()) {
        case GameObjectGroup::TERRAIN:
            for (auto& pointer : _objectsTerrain)
                if (&*pointer == object) {
                    pointer.reset();
                    return;
                }
            break;

        case GameObjectGroup::ENTITY:
            for (auto& pointer : _objectsEntity)
                if (&*pointer == object) {
                    pointer.reset();
                    return;
                }
            break;
    }
}