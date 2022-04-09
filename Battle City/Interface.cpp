#include "Interface.h"
#include "Game.h"


Interface::Interface(class Game& game) : _game(game) {
    // Загрузка шрифтов
    _debugFont.reset(new sf::Font);
    if (!_debugFont->loadFromFile("./Build/progresspixel-bold.ttf"))
        if (!_debugFont->loadFromFile("./build/progresspixel-bold.ttf"))
            if (!_debugFont->loadFromFile("progresspixel-bold.ttf"))
                std::exit(1);

    _objectsDataFont.reset(new sf::Font);
    if (!_objectsDataFont->loadFromFile("./Build/CursedTimerUlil.ttf"))
        if (!_objectsDataFont->loadFromFile("./build/CursedTimerUlil.ttf"))
            if (!_objectsDataFont->loadFromFile("CursedTimerUlil.ttf"))
                std::exit(1);
}

void Interface::render() {
    std::string string;
    sf::Text text;
    sf::RectangleShape rectangle;

    text.setFont(*_debugFont);
    text.setCharacterSize(24);
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