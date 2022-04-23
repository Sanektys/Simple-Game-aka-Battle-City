#include "Interface.h"
#include "Game.h"


Interface::Interface(class Game& game) : _game(game) {
    // Загрузка шрифтов
    if (!_debugFont.loadFromFile("./Build/progresspixel-bold.ttf"))
        if (!_debugFont.loadFromFile("./build/progresspixel-bold.ttf"))
            _debugFont.loadFromFile("progresspixel-bold.ttf");

    if (!_objectsDataFont.loadFromFile("./Build/CursedTimerUlil.ttf"))
        if (!_objectsDataFont.loadFromFile("./build/CursedTimerUlil.ttf"))
            _objectsDataFont.loadFromFile("CursedTimerUlil.ttf");

    unsigned screenWidth{_game._renderWindow->getSize().x};
    unsigned screenHeight{_game._renderWindow->getSize().y};

    ///////////////////////////////////////////////////////
    // Миникарта
    _mapLayout.reset(new sf::RenderTexture);
    // Размер всего игрового поля
    _mapLayout->create(level::COLUMNS * level::PIXELS_PER_CELL,
                       level::ROWS    * level::PIXELS_PER_CELL);

    // Масштабирование слоя миникарты в меньшую сторону
    float mapScale = (_game._renderWindow->getSize().x / 5.5f) / _mapLayout->getSize().x;
    _map.setScale(mapScale, mapScale);
    _map.setPosition(20.0f, 40.0f);
    _map.setTexture(_mapLayout->getTexture());

    _mapFrame.setSize(sf::Vector2f(level::COLUMNS * level::PIXELS_PER_CELL * mapScale,
                                   level::ROWS    * level::PIXELS_PER_CELL * mapScale));
    _mapFrame.setPosition(_map.getPosition());
    _mapFrame.setFillColor(sf::Color(0, 0, 0));
    _mapFrame.setOutlineThickness(4.0f);
    _mapFrame.setOutlineColor(sf::Color(255, 197, 85));

    ///////////////////////////////////////////////////////
    // Показатель перезарядки танка
    _rechargeFrame.setSize(sf::Vector2f(screenWidth  / 4.0f,
                                        screenHeight / 76.0f));
    _rechargeFrame.setPosition(screenWidth / 2.0f - _rechargeFrame.getSize().x / 2.0f,
                               screenHeight - _rechargeFrame.getSize().y - 25.0f);
    _rechargeFrame.setFillColor(sf::Color(30, 30, 30, 230));
    _rechargeFrame.setOutlineThickness(2.0f);
    _rechargeFrame.setOutlineColor(sf::Color(200, 176, 55, 200));

    _rechargeBar.setSize(_rechargeFrame.getSize());
    _rechargeBar.setPosition(_rechargeFrame.getPosition());
    _rechargeBar.setFillColor(sf::Color(220, 210, 56, 140));

    ///////////////////////////////////////////////////////
    // Показатель прочности базы
    _baseHealthColor = sf::Color(128, 255, 0, 100);
    _baseHealthFrame.setSize(sf::Vector2f(screenWidth  / 5.5f,
                                          screenHeight / 26.0f));
    _baseHealthFrame.setPosition(screenWidth / 2.0f - _baseHealthFrame.getSize().x - 40.0f,
                                 screenHeight - _baseHealthFrame.getSize().y
                                              - _rechargeFrame.getSize().y - 55.0f);
    _baseHealthFrame.setFillColor(sf::Color(50, 50, 50, 230));
    _baseHealthFrame.setOutlineThickness(4.0f);
    _baseHealthFrame.setOutlineColor(_baseHealthColor);

    _baseHealthBar.setFillColor(sf::Color(255, 82, 16, 160));
    _baseHealthBar.setSize(_baseHealthFrame.getSize());
    _baseHealthBar.setPosition(_baseHealthFrame.getPosition());

    _baseHealthText.setFillColor(_baseHealthColor);
    _baseHealthText.setFont(_objectsDataFont);
    _baseHealthText.setString("BASE HEALTH");
    _baseHealthText.setCharacterSize(unsigned(_baseHealthFrame.getSize().y) / 10u * 8u);
    _baseHealthText.setLetterSpacing(2.0f);
    _baseHealthText.setOrigin(_baseHealthText.getGlobalBounds().width  / 2.0f,
                              _baseHealthText.getGlobalBounds().height / 2.0f);
    _baseHealthText.setPosition(_baseHealthFrame.getPosition().x
                                + _baseHealthFrame.getSize().x / 2.0f,
                                _baseHealthFrame.getPosition().y
                                + _baseHealthFrame.getSize().y / 3.0f);

    ///////////////////////////////////////////////////////
    // Показатель прочности танка игрока
    _tankHealthColor = sf::Color(128, 255, 0, 100);
    _tankHealthFrame.setSize(sf::Vector2f(screenWidth  / 5.5f,
                                          screenHeight / 26.0f));
    _tankHealthFrame.setPosition(screenWidth / 2.0f + 40.0f,
                                 screenHeight - _tankHealthFrame.getSize().y
                                              - _rechargeFrame.getSize().y - 55.0f);
    _tankHealthFrame.setFillColor(sf::Color(50, 50, 50, 230));
    _tankHealthFrame.setOutlineThickness(4.0f);
    _tankHealthFrame.setOutlineColor(_tankHealthColor);

    _tankHealthBar.setFillColor(sf::Color(88, 85, 0, 160));
    _tankHealthBar.setSize(_tankHealthFrame.getSize());
    _tankHealthBar.setPosition(_tankHealthFrame.getPosition());

    _tankHealthText.setFillColor(_tankHealthColor);
    _tankHealthText.setFont(_objectsDataFont);
    _tankHealthText.setString("TANK HEALTH");
    _tankHealthText.setCharacterSize(unsigned(_tankHealthFrame.getSize().y) / 10u * 8u);
    _tankHealthText.setLetterSpacing(2.0f);
    _tankHealthText.setOrigin(_tankHealthText.getGlobalBounds().width  / 2.0f,
                              _tankHealthText.getGlobalBounds().height / 2.0f);
    _tankHealthText.setPosition(_tankHealthFrame.getPosition().x
                                + _baseHealthFrame.getSize().x / 2.0f,
                                _tankHealthFrame.getPosition().y
                                + _tankHealthFrame.getSize().y / 3.0f);
}

void Interface::renderMap() {
    if (_mapUpdateTimer >= MAP_UPDATE_PERIOD) {
        _mapUpdateTimer = 0.0f;

        _mapLayout->clear();
        for (auto& object : _game._objectsTerrain)
            if (object)
                object->mapRender(&*_mapLayout);
        for (auto& object : _game._objectsEntity)
            if (object)
                object->mapRender(&*_mapLayout);
        _mapLayout->display();
    }

    _game._renderWindow->draw(_mapFrame);
    _game._renderWindow->draw(_map);
}

void Interface::renderStats() {
    showBaseHealth();
    showTankHealth();
    showTankRecharge();

    std::string string;
    sf::Text text;
    sf::RectangleShape rectangle;

    text.setFont(_debugFont);
    text.setCharacterSize(16);
    text.setLetterSpacing(1.6f);

    int objectsCount{0};


    // Счётчик обновлений в секунду
    string = "UPS: " + std::to_string(_game._ups);

    text.setString(string);
    text.setPosition(float(_game._renderWindow->getSize().x)
                     - text.getGlobalBounds().width, 2.0f);
    text.setFillColor(sf::Color(165, 92, 126, 255));

    rectangle.setSize(sf::Vector2f(text.getGlobalBounds().width + 8.0f,
                      text.getGlobalBounds().height + 8.0f));
    rectangle.setPosition(sf::Vector2f(text.getGlobalBounds().left - 4.0f,
                          text.getGlobalBounds().top - 4.0f));
    rectangle.setFillColor(sf::Color(20, 20, 20, 220));

    _game._renderWindow->draw(rectangle);
    _game._renderWindow->draw(text);


#ifdef _DEBUG
    // Текущее разрешение
    string = std::to_string(sf::VideoMode::getDesktopMode().width) + "x"
             + std::to_string(sf::VideoMode::getDesktopMode().height);

    text.setString(string);
    text.setPosition(0.0f, 0.0f);
    text.setFillColor(sf::Color(125, 155, 185, 200));

    rectangle.setSize(sf::Vector2f(text.getGlobalBounds().width + 8.0f,
                      text.getGlobalBounds().height + 8.0f));
    rectangle.setPosition(sf::Vector2f(text.getGlobalBounds().left - 4.0f,
                          text.getGlobalBounds().top - 4.0f));
    rectangle.setFillColor(sf::Color(20, 20, 20, 220));

    _game._renderWindow->draw(rectangle);
    _game._renderWindow->draw(text);


    // Счётчик объектов в игре
    objectsCount = _game.getObjectsCount(GameObjectType::ALL);
    string = "Objects: " + std::to_string(objectsCount);

    text.setString(string);
    text.setPosition(float(_game._renderWindow->getSize().x)
                     - text.getGlobalBounds().width, 34.0f);
    text.setFillColor(sf::Color(125, 155, 185, 200));

    rectangle.setSize(sf::Vector2f(text.getGlobalBounds().width + 8.0f,
                      text.getGlobalBounds().height + 8.0f));
    rectangle.setPosition(sf::Vector2f(text.getGlobalBounds().left - 4.0f,
                          text.getGlobalBounds().top - 4.0f));
    rectangle.setFillColor(sf::Color(20, 20, 20, 220));

    _game._renderWindow->draw(rectangle);
    _game._renderWindow->draw(text);


    // Счётчик уничтоженных и живых противников
    objectsCount = _game.getObjectsCount(GameObjectType::TANK_ENEMY);
    string = "Destroyed enemies: " + std::to_string(_game.getDiedEnemiesCount())
             + "\nLiving enemies: " + std::to_string(objectsCount)
             + "\nTotal on level: " + std::to_string(level::tank::enemy::PER_LEVEL);

    text.setString(string);
    text.setPosition(float(_game._renderWindow->getSize().x)
                     - text.getGlobalBounds().width, 70.0f);
    text.setFillColor(sf::Color(165, 92, 126, 255));

    rectangle.setSize(sf::Vector2f(text.getGlobalBounds().width + 8.0f,
                      text.getGlobalBounds().height + 8.0f));
    rectangle.setPosition(sf::Vector2f(text.getGlobalBounds().left - 4.0f,
                          text.getGlobalBounds().top - 4.0f));
    rectangle.setFillColor(sf::Color(20, 20, 20, 220));

    _game._renderWindow->draw(rectangle);
    _game._renderWindow->draw(text);
#endif
}

void Interface::showBaseHealth() {
    _game._renderWindow->draw(_baseHealthFrame);    
    _game._renderWindow->draw(_baseHealthText);
    _game._renderWindow->draw(_baseHealthBar);
}

void Interface::showTankHealth() {
    _game._renderWindow->draw(_tankHealthFrame);
    _game._renderWindow->draw(_tankHealthText);
    _game._renderWindow->draw(_tankHealthBar);
}

void Interface::showTankRecharge() {
    _game._renderWindow->draw(_rechargeFrame);
    _game._renderWindow->draw(_rechargeBar);
}