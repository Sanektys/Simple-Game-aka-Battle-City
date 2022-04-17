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
namespace level {
    sf::Texture* ATLAS_TERRAIN{nullptr};
    sf::Texture* ATLAS_ENTITY{nullptr};
}
std::mt19937* RANDOM{nullptr};


Game::~Game() {
    if (level::ATLAS_TERRAIN)
        delete level::ATLAS_TERRAIN;
    if (level::ATLAS_ENTITY)
        delete level::ATLAS_ENTITY;
    if (RANDOM)
        delete RANDOM;
}

void Game::setupSystem() {
    // ��������� ����� �������
    RANDOM = new std::mt19937;
    RANDOM->seed(unsigned(steady_clock::now().time_since_epoch().count()));

    // �������� �������� ����
    _renderWindow.reset(new sf::RenderWindow(sf::VideoMode::getDesktopMode(),
                                             "Battle City", sf::Style::Fullscreen));
    _renderWindow->setMouseCursorVisible(false);

    // ������������ ������ ������
    _playerCamera.reset(new sf::View(
        sf::FloatRect(0.0f, 0.0f,
                      level::CAMERA_WIDTH, level::CAMERA_HEIGHT)));

    // �������� �������, ����������� �� ���������
    _interface.reset(new Interface(*this));

    // �������� ������� �� �������
    level::ATLAS_TERRAIN = new sf::Texture();
    if (!level::ATLAS_TERRAIN->loadFromFile("./Build/atlas_terrain.png"))
        if (!level::ATLAS_TERRAIN->loadFromFile("./build/atlas_terrain.png"))
            level::ATLAS_TERRAIN->loadFromFile("atlas_terrain.png");

    level::ATLAS_ENTITY = new sf::Texture();
    if (!level::ATLAS_ENTITY->loadFromFile("./Build/atlas_entity.png"))
        if (!level::ATLAS_ENTITY->loadFromFile("./build/atlas_entity.png"))
            level::ATLAS_ENTITY->loadFromFile("atlas_entity.png");
}

void Game::initialize() {
    shutdown();

    _clockLastFrame = steady_clock::now();
    _diedEnemiesCount = 0;

    // �������� ������
    for (int r = 0; r < level::ROWS; r++) {
        for (int c = 0; c < level::COLUMNS; c++) {
            unsigned char cellSymbol = level::FIRST_MAP[r][c];
            std::unique_ptr<GameObject>* terrainObject{nullptr};

            switch (cellSymbol) {
                case level::SYMBOL_BRICK_WALL :
                    terrainObject = &createObject(GameObjectType::WALL, (float)c, (float)r);
                    break;

                case level::SYMBOL_SOLID_WALL :
                    terrainObject = &createObject(GameObjectType::SOLID_WALL, (float)c, (float)r);
                    break;
                    
                case level::SYMBOL_STEEL_WALL :
                    terrainObject = &createObject(GameObjectType::STEEL_WALL, (float)c, (float)r);
                    break;
    
                case level::SYMBOL_BASE :
                    terrainObject = _base = &createObject(GameObjectType::BASE, (float)c, (float)r);
                    break;
    
                case level::SYMBOL_PLAYER_1 :
                    //_playerOne = &createObject(
                    //    GameObjectType::TANK_FIRST_PLAYER, (float)c, (float)r);
                    break;
    
                case level::SYMBOL_PLAYER_2 :
                    _playerTwo = &createObject(
                        GameObjectType::TANK_SECOND_PLAYER, (float)c, (float)r);
                    break;
    
                case level::SYMBOL_ENEMY_SPAWNER :
                    createObject(GameObjectType::ENEMY_SPAWNER, (float)c, (float)r);
                    break;
            }
            addCollisionCell(terrainObject, r * level::COLUMNS + c);
        }
    }
}

void Game::addCollisionCell(std::unique_ptr<GameObject>* owner, int order) {
    if (owner != nullptr && *owner) {
        int row{0}, column{0};
        int x{0}, y{0};

        // �������� ���� �������� �������� ��������� �������������
        // �������������, �������� �� ���� ������� �������, ��������� ���
        // ���������� �� �������� �������� ���� � ��������� �� �� ���
        while (++row <= int((*owner)->getHeight())) {
            while (++column <= int((*owner)->getWidth())) {
                x = int((*owner)->getX()) + column - 1;
                y = int((*owner)->getY()) + row - 1;

                // ������ ������� - ��������� �������� � ������������
                _terrainCollision[y * level::COLUMNS + x] = owner;
            }
            column = 0;
        }
    }
}

bool Game::loop() {
    // ��������� �������� �������� ����
    if (!_renderWindow->isOpen())
        return false;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        _renderWindow->close();
        return false;
    }

    using std::chrono::duration;
    using std::chrono::duration_cast;
    steady_clock::time_point clockNow;

    // ���������� ���� �� �����
    bool wasPause{false};
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Pause)) {
        wasPause = true;
        clockNow = steady_clock::now();
        while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) &&
               !sf::Keyboard::isKeyPressed(sf::Keyboard::Space));        
    }

    // ������ ���������� ������� �� ����
    if (!wasPause)
        clockNow = steady_clock::now();

    float deltaTime =
        duration_cast<duration<float>>(clockNow - _clockLastFrame).count();

    if (!wasPause)
        _clockLastFrame = clockNow;
    else
        _clockLastFrame = steady_clock::now();

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
    for (auto& pointer : _objectsTerrain)
        if (pointer)
            pointer.reset();

    for (auto& pointer : _objectsEntity)
        if (pointer)
            pointer.reset();

    for (auto& cell : _terrainCollision)
        cell = nullptr;
}

void Game::render() {
    // ������ �����
    _renderWindow->clear(sf::Color(20, 20, 20));

    // ���������� ��������� ������ ����� �� ������ ������
    // ����� ���� �������� ����� ���������� ������� ��������,
    // ����� ��� ���� ��������� � ��������� ������������ ������������ �����
    _playerCamera->setCenter(((*_playerTwo)->getX()
                             + (*_playerTwo)->getWidth() / 2.0f) * level::PIXELS_PER_CELL,
                             ((*_playerTwo)->getY()
                             + (*_playerTwo)->getHeight() / 2.0f) * level::PIXELS_PER_CELL);
    _renderWindow->setView(*_playerCamera);

    // ��������� ���� ������� ��������
    for (auto& pointer : _objectsTerrain)
        if (pointer)
            pointer->render(&*_renderWindow);
    for (auto& pointer : _objectsEntity)
        if (pointer)
            pointer->render(&*_renderWindow);
    
    // ����� ���� �� ���������� ��������
    // ��, ��� ��� ������, �������������� ������������ �������� ������
    _renderWindow->setView(_renderWindow->getDefaultView());

    _interface->render();

    // ����� �����
    _renderWindow->display();
}

void Game::update(float dt) {
    // ���������� ������ ���� ������������ ��������
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

    // ��� ����������� ����
    if (*_base && (*_base)->getHealth() <= 0)
        initialize();

    // ����������� �������
    //if (*_playerOne && (*_playerOne)->getHealth() <= 0)
    //    _playerOne->reset();

    if (*_playerTwo && (*_playerTwo)->getHealth() <= 0)
        initialize();

    // ��� ���������� ����������
    if (_diedEnemiesCount == level::tank::enemy::PER_LEVEL)
        initialize();
}

std::unique_ptr<GameObject>& Game::checkIntersects(
    float x, float y, float width, float height,
    class GameObject* exceptObject, enum GameObjectGroup group) const {
    static std::unique_ptr<GameObject> returnObject;

    // ����� ������� ���� �������� �������
    float primaryCoordY = y;
    float primaryCoordX = x;
    // ������ ������ ���� �������� �������
    float overallCoordY = primaryCoordY + height - 0.00001f;
    float overallCoordX = primaryCoordX + width  - 0.00001f;

    if (GameObjectGroup::ALL == group || GameObjectGroup::TERRAIN == group) {
        std::unique_ptr<GameObject>* pointer{nullptr};
        // ��������, ������������ � ������ ������� ���������� ����������� ���������,
        // �� ������� ��������� �������� ������������ ����� �������
        unsigned primaryY{unsigned(y) * level::COLUMNS};
        // ��������, ������������ �� ����� ������� ��������� ������ ������ ���� �������
        unsigned finalCoordX{unsigned(overallCoordY) * level::COLUMNS
                             + unsigned(overallCoordX)};

        for (size_t i{primaryY + unsigned(x)}; ; ) {
            // ���� ������ �������� �� ���� �� ������ ������� ����� �������
            if (i <= primaryY + unsigned(overallCoordX)) {
                pointer = _terrainCollision[i];
                if (pointer != nullptr && *pointer
                    && &**pointer != exceptObject && (**pointer).getPhysical()) {
                    return *pointer;  // ������� ������-������
                } else {
                    i++;
                    continue;
                }
            } else if (i <= finalCoordX) {
                primaryY += level::COLUMNS;  // ������� �� ��������� ����������� ���������
                i = primaryY + unsigned(x);
            } else
                break;  // �������-������ ���, ����������� �����������
        }
    }
    if (GameObjectGroup::ALL == group || GameObjectGroup::ENTITY == group) {
        float pcY{0.0f}, pcX{0.0f}, ocY{0.0f}, ocX{0.0f};
        bool conditionOne  {false}, conditionTwo {false},
             conditionThree{false}, conditionFour{false};

        for (auto& pointer : _objectsEntity)
            if (pointer && &*pointer != exceptObject && pointer->getPhysical()) {
                pcY = pointer->getY();
                pcX = pointer->getX();
                ocY = pcY + pointer->getHeight() - 0.00001f;
                ocX = pcX + pointer->getWidth()  - 0.00001f;

                conditionOne   = primaryCoordY <= ocY;
                conditionTwo   = overallCoordY >= pcY;
                conditionThree = primaryCoordX <= ocX;
                conditionFour  = overallCoordX >= pcX;

                if (conditionOne && conditionTwo && conditionThree && conditionFour)
                    return pointer;  // ��� ����������� ������� ��������� ������-������
            }
    }

    return returnObject;
}

bool Game::moveObjectTo(class GameObject* object, float x, float y) const {
    // ���������� ��������(�����) �������
    float newCoordY = y;
    float newCoordX = x;
    // �������� ������� �� ����� �������
    float newOverallCoordY = newCoordY + object->getHeight() - 0.00001f;
    float newOverallCoordX = newCoordX + object->getWidth() - 0.00001f;

    // �������� �� �������� ���������� (����� �� ������� �������� ����)
    bool conditionOne   = newCoordY < 0.0f;
    bool conditionTwo   = newCoordX < 0.0f;
    bool conditionThree = newOverallCoordY >= level::ROWS;
    bool conditionFour  = newOverallCoordX >= level::COLUMNS;

    if (conditionOne || conditionTwo || conditionThree || conditionFour)
        return false;

    // �������� �� ����������� � ������ �������� �����
    // ������� ������ ��� �� ����� �������
    std::unique_ptr<GameObject>& otherObject{checkIntersects(x, y, object->getWidth(),
                                                             object->getHeight(), object)};

    // ���� ��������� ���������(���� ����������� � ������ ��������),
    // �� ��������� ��������� ��� ������������
    if (otherObject) {
        object->intersect(&*otherObject);
        object->setYSpeed(0.0f);
        object->setXSpeed(0.0f);
        return false;
    } else {    //����� ��������� ������ �� ����� �������
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
    else if (GameObjectType::ALL == type) {
        for (auto& pointer : _objectsTerrain)
            if (pointer)
                totalCount++;
        for (auto& pointer : _objectsEntity)
            if (pointer)
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
            dynamic_cast<TankPlayer&>(*object).setAltKeys(
                sf::Keyboard::Left, sf::Keyboard::Right,
                sf::Keyboard::Up,   sf::Keyboard::Down,
                sf::Keyboard::Space);
            break;

        case GameObjectType::TANK_ENEMY :
            // ��������������� �������� ����, ��� ��� ������ ��� ������ � �������� 10�10
            if (!checkIntersects(x - 4.0f, y - 4.0f, 10.0f, 10.0f, nullptr,
                                 GameObjectGroup::ENTITY))
                object.reset(new TankEnemy(*this, level::tank::enemy::basic::IMAGE));
            break;

        case GameObjectType::BULLET :
            object.reset(new Bullet(*this, level::bullet::basic::IMAGE));
            break;

        case GameObjectType::ENEMY_SPAWNER :
            object.reset(new EnemySpawner(*this));
            break;

        default :
            break;
    }

    if (!object)
        return object;

    if (moveObjectTo(&*object, x, y) == false) {
        object.reset();
        return object;
    }

    // ���������� ���������� ��������� � ���������� ������ ���������� ������� ��������
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

        default :
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

        default :
            break;
    }
}
