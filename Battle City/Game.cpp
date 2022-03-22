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
// ���������� ����������
sf::Texture* _atlasTerrain;
sf::Texture* _atlasEntity;
std::mt19937 random;


Game::~Game() {
	if (_debugFont) {
		delete _debugFont;
		_debugFont = nullptr;
	}

	if (_atlasEntity) {
		delete _atlasEntity;
		_atlasEntity = nullptr;
	}

	if (_atlasTerrain) {
		delete _atlasTerrain;
		_atlasTerrain = nullptr;
	}

	if (_renderWindow) {
		delete _renderWindow;
		_renderWindow = nullptr;
	}
}

void Game::setupSystem() {
	random.seed(steady_clock::now().time_since_epoch().count());

	_renderWindow = new sf::RenderWindow(
		sf::VideoMode(PIXELS_PER_CELL * SCREEN_COLUMNS,
		              PIXELS_PER_CELL * SCREEN_ROWS, 32),
		"Battle City", sf::Style::Titlebar | sf::Style::Close);

	_atlasTerrain = new sf::Texture();
	_atlasTerrain->loadFromFile("atlas_terrain.png");
	_atlasEntity = new sf::Texture();
	_atlasEntity->loadFromFile("atlas_entity.png");

	_debugFont = new sf::Font;
	_debugFont->loadFromFile("progresspixel-bold.ttf");
}

void Game::initialize() {
	shutdown();

	_diedEnemiesCount = 0;

	// �������� ������
	for (int r = 0; r < LEVEL_ROWS; r++) {
		for (int c = 0; c < LEVEL_COLUMNS; c++) {
			unsigned char cellSymbol = FIRST_LEVEL_DATA[r][c];

			switch (cellSymbol) {
			    case CELL_SYMBOL_BRICK_WALL : {
				    class Wall* wall = dynamic_cast<Wall*>(createObject(
						GameObjectType::WALL, (float)c, (float)r));
					if (wall) {
						wall->setTextureRect(BRICK_WALL_IMAGE);
					}
					break;
			    }
				case CELL_SYMBOL_SOLID_WALL : {
					class Wall* wall = dynamic_cast<Wall*>(createObject(
						GameObjectType::SOLID_WALL, (float)c, (float)r));
					if (wall) {
					    wall->Wall::setTextureRect(SOLID_BRICK_WALL_IMAGE);
					}
					break;
				}				
			    case CELL_SYMBOL_STEEL_WALL : {
				    class Wall* wall = dynamic_cast<Wall*>(createObject(
						GameObjectType::WALL, (float)c, (float)r));
					if (wall) {
						wall->setTextureRect(STEEL_WALL_IMAGE);
						wall->setInvulnerable(true);
					}
				    break;
			    }
				case CELL_SYMBOL_BASE : {
					_base = createObject(
						GameObjectType::BASE, (float)c, (float)r);
					if (_base) {
						_base->setTextureRect(BASE_IMAGE);
					}
					break;
				}
				case CELL_SYMBOL_PLAYER_1 : {
					_playerOne = dynamic_cast<TankPlayer*>(createObject(
						GameObjectType::TANK_FIRST_PLAYER, (float)c, (float)r));
					if (_playerOne) {
						_playerOne->setTextureRect(FIRST_PLAYER_TANK_IMAGE);
						dynamic_cast<TankPlayer*>(_playerOne)
							->setKeys(VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_SPACE);
					}
					break;
				}
				case CELL_SYMBOL_PLAYER_2 : {
					_playerTwo = dynamic_cast<TankPlayer*>(createObject(
						GameObjectType::TANK_SECOND_PLAYER, (float)c, (float)r));
					if (_playerTwo) {
						_playerTwo->setTextureRect(SECOND_PLAYER_TANK_IMAGE);
						dynamic_cast<TankPlayer*>(_playerTwo)
							->setKeys('A', 'D', 'W', 'S', 'E');
					}
					break;
				}
				case CELL_SYMBOL_ENEMY_SPAWNER :
					//createObject(GameObjectType::ENEMY_SPAWNER, (float)c, (float)r);
					break;
			}
		}
	}
}

bool Game::loop() {
	if (!_renderWindow->isOpen())
		return false;

	// ������ ���������� ������� �� ����
	using std::chrono::duration;
	using std::chrono::duration_cast;

	steady_clock::time_point clockNow = steady_clock::now();
	float deltaTime = 
		duration_cast<duration<float>>(clockNow - _clockLastFrame).count();
	_clockLastFrame = clockNow;

	// ������� ���������� � ������� (������ � �������)
	_oneSecond += deltaTime;
	_updatesCount++;
	if (_oneSecond >= 1.0f) {
		_oneSecond = 0.0f;
		_ups = _updatesCount;
		_updatesCount = 0;
	}

	// ������������ �������� ����
	sf::Event event;
	while (_renderWindow->pollEvent(event))
		if (event.type == sf::Event::Closed)
			_renderWindow->close();

	update(deltaTime);
	render();

	return _isGameActive;
}

void Game::shutdown() {
	for (int i = 0; i < OBJECTS_COUNT_MAX; i++)
		if (_objects[i] != 0) {
			delete _objects[i];
			_objects[i] = nullptr;
		}
}

void Game::render() {
	std::string string;
	sf::Text text;
	text.setFont(*_debugFont);
	text.setCharacterSize(24);
	text.setLetterSpacing(1.6f);
	
	// ������ �����
	_renderWindow->clear(sf::Color(20, 20, 20));

	// ��������� ���� ������� ��������
	int objectsCount = 0;
	for (int i = 0; i < OBJECTS_COUNT_MAX; i++)
		if (_objects[i] != 0) {
			_objects[i]->render(_renderWindow);
			objectsCount++;
		}

	// ������� ���������� � �������
	string = "UPS: " + std::to_string(_ups);
	text.setString(string);
	text.setPosition(2250, 2);
	text.setFillColor(sf::Color::Color(165, 92, 126, 255));
	_renderWindow->draw(text);
	
	// ������� �������� � ����
    #ifdef _DEBUG
	string = "Objects: " + std::to_string(objectsCount);
	text.setString(string);
	text.setPosition(2250, 34);
	text.setFillColor(sf::Color::Color(125, 155, 185, 200));
	_renderWindow->draw(text);
    #endif

	// ����� �����
	_renderWindow->display();
}

void Game::update(float dt) {
	// ��� ����������� ����
	if (_base && _base->getHealth() <= 0)
		initialize();
	
	for (int i = 0; i < OBJECTS_COUNT_MAX; i++)
		if (_objects[i] != 0) {
			_objects[i]->update(dt);

			if (_objects[i]->getHealth() <= 0
				&& _objects[i]->getDestroyAfterDeath()) 
				destroyObject(i);
		}

	// ����������� �������
	if (_playerOne && _playerOne->getHealth() <= 0) {
		destroyObject(_playerOne);
		_playerOne = nullptr;
	}

	if (_playerTwo && _playerTwo->getHealth() <= 0) {
		destroyObject(_playerTwo);
		_playerTwo = nullptr;
	}

	// ��� ���������� ����������
	if (_diedEnemiesCount == ENEMIES_PER_LEVEL)
		initialize();
}

// �������� ����������� ������ ������� ������
GameObject* Game::checkIntersects(float x, float y, float width, float height,
	                              class GameObject* exceptObject) {
	// ����� ������� ���� �������� �������
	float primaryCoordY = y;
	float primaryCoordX = x;
	// ������ ������ ���� �������� �������
	float overallCoordY = primaryCoordY + height - 0.00001f;
	float overallCoordX = primaryCoordX + width - 0.00001f;
	
	for (int i = 0; i < OBJECTS_COUNT_MAX; i++) {
		
		bool conditionOne = _objects[i] != 0;
		bool conditionTwo = _objects[i] != exceptObject;
		
		if (conditionOne && conditionTwo && _objects[i]->getPhysical()) {
			float pcY = _objects[i]->getY();
			float pcX = _objects[i]->getX();
			float ocY = pcY + _objects[i]->getHeight() - 0.00001f;
			float ocX = pcX + _objects[i]->getWidth() - 0.00001f;
			
			bool conditionOne   = primaryCoordY <= ocY;
			bool conditionTwo   = overallCoordY >= pcY;
			bool conditionThree = primaryCoordX <= ocX;
			bool conditionFour  = overallCoordX >= pcX;
			
			if (conditionOne && conditionTwo && conditionThree && conditionFour)
				return _objects[i];    // ��� ����������� ������� ��������� ������-������
		}
	}

	return nullptr;
}

// �������� �� ����������� ������� ������������� �� ������� x,y
// ���� ����������� ��������, �� ������ ��������������� �� x,y
bool Game::moveObjectTo(class GameObject* object, float x, float y) {
	// ���������� ��������(�����) �������
	float newCoordY = y;
	float newCoordX = x;
	// �������� ������� �� ����� �������
	float newOverallCoordY = newCoordY + object->getHeight() - 0.00001f;
	float newOverallCoordX = newCoordX + object->getWidth() - 0.00001f;

	// �������� �� �������� ���������� (����� �� ������� �������� ����)
	bool conditionOne   = newCoordY < 0.0f;
	bool conditionTwo   = newCoordX < 0.0f;
	bool conditionThree = newOverallCoordY >= LEVEL_ROWS;
	bool conditionFour  = newOverallCoordX >= LEVEL_COLUMNS;
	
	if (conditionOne || conditionTwo || conditionThree || conditionFour)
		return false;

	// �������� �� ����������� � ������ �������� �����
	// ������� ������ ��� �� ����� �������
	class GameObject* otherObject = checkIntersects(x, y, object->getWidth(),
													object->getHeight(), object);

	// ���� ��������� ���������(���� ����������� � ������ ��������),
	// �� ��������� ��������� ��� ������������
	if (otherObject) {
		object->intersect(otherObject);
		object->setYSpeed(0.0f);
		object->setXSpeed(0.0f);
		return false;
	} else {    //����� ��������� ������ �� ����� �������
		object->setX(x);
		object->setY(y);
		return true;
	}

	return false;
}

// �������� ���� �������� ������������ ����
int Game::getObjectsCount(enum GameObjectType type) {
	int totalCount = 0;

	for (int i = 0; i < OBJECTS_COUNT_MAX; i++)
		if (_objects[i] != 0 && _objects[i]->getType() == type)
			totalCount++;

	return totalCount;
}

GameObject* Game::createObject(enum GameObjectType type, float x, float y) {
	// ���������� ���������� ��������� � ���������� ������ ���������� ������� ��������
	for (int i = 0; i < OBJECTS_COUNT_MAX; i++) {
		if (_objects[i] == 0) {
			class GameObject* object = nullptr;

			switch (type) {
			    case GameObjectType::WALL :
				    object = new Wall();
				    break;

				case GameObjectType::SOLID_WALL :
					object = new SolidWall();
					break;
				
				case GameObjectType::BASE :
					object = new Base();
					break;
				
				case GameObjectType::TANK_FIRST_PLAYER :
				case GameObjectType::TANK_SECOND_PLAYER :
					object = new TankPlayer();
					break;
				
				case GameObjectType::TANK_ENEMY :
					object = new TankEnemy();
					break;
				
				case GameObjectType::BULLET :
					object = new Bullet();
					break;
				
				case GameObjectType::ENEMY_SPAWNER :
					object = new EnemySpawner();
					break;
			}

			if (object == nullptr)
				return nullptr;

			if (moveObjectTo(object, x, y) == false) {
				delete object;
				object = nullptr;
				return nullptr;
			}

			object->setGame(this);
			_objects[i] = object;

			return object;
		}
	}

	return nullptr;
}

void Game::destroyObject(const class GameObject* object) {
	for (int i = 0; i < OBJECTS_COUNT_MAX; i++) {
		if (_objects[i] == object) {
			delete _objects[i];
			_objects[i] = nullptr;

			return;
		}
	}
}