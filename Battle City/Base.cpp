#include "Base.h"
#include "GameObjectType.h"
#include "Level.h"


Base::Base(const class Game& game, sf::IntRect rect) : GameObject(game) {
    setGroup(GameObjectGroup::TERRAIN);
    setType(GameObjectType::BASE);

    setWidth(level::base::COLUMNS);
    setHeight(level::base::ROWS);

    setHealth(level::base::HEALTH);
    setDestroyAfterDeath(false);

    _spriteTerrain.reset(new sf::Sprite());
    _spriteTerrain->setTexture(*level::ATLAS_TERRAIN);
    _spriteTerrain->setOrigin(getWidth()  * level::PIXELS_PER_CELL / 2.0f,
                              getHeight() * level::PIXELS_PER_CELL / 2.0f);

    setTextureRect(rect);
}

void Base::render(sf::RenderWindow* rw) {
    setTextureRect(_spriteTerrain->getTextureRect());

    GameObject::render(rw);
}

void Base::mapRender(sf::RenderTexture* rt) {
    rt->draw(_rectanglePinOnMap);
    rt->draw(_mapPinText);
}

void Base::setMapPin() {
    _rectanglePinOnMap.setSize(sf::Vector2f(getWidth()  * level::PIXELS_PER_CELL,
                                            getHeight() * level::PIXELS_PER_CELL));
    _rectanglePinOnMap.setPosition(getX() * level::PIXELS_PER_CELL,
                                   getY() * level::PIXELS_PER_CELL);
    _rectanglePinOnMap.setFillColor(sf::Color(255, 197, 85, 60));

    // �������� ������ ��� ������� "����" �� ���������
    if (!_mapPinFont.loadFromFile("./Build/progresspixel-bold.ttf"))
        if (!_mapPinFont.loadFromFile("./build/progresspixel-bold.ttf"))
            _mapPinFont.loadFromFile("progresspixel-bold.ttf");

    _mapPinText.setFont(_mapPinFont);
    _mapPinText.setCharacterSize(60);
    _mapPinText.setLetterSpacing(2.8f);
    _mapPinText.setFillColor(sf::Color(255, 197, 85, 220));
    _mapPinText.setString("BASE");
    _mapPinText.setPosition(getX() * level::PIXELS_PER_CELL + 22.0f,
                            getY() * level::PIXELS_PER_CELL - 1.0f);
}

void Base::update(float dt) {
    // �������� �� ������������ ������� ����
    _frameSwitchTime += dt;
    if (0.25f <= _frameSwitchTime)
        _frameSwitchTime = 0.0f;   // ����� ���� - ������� ������
}

void Base::setTextureRect(sf::IntRect rect) {
    // ��������������� �������� �� ����������� � ������ ��� ��������
    if (0.0f == _frameSwitchTime) {
        if (_toLeft) {
            _horizontalFrameNumber++;
            if (_horizontalFrameNumber == 3)
                _toLeft = false;
        } else {
            _horizontalFrameNumber--;
            if (_horizontalFrameNumber == 1)
                _toLeft = true;
        }
    }
    // ������������ �������� (�� ���������) �� ������ ���������� ���������
    switch (getHealth()) {
        case 8 :
        case 7 :
            rect.top = 2 * level::PIXELS_PER_CELL;
            break;

        case 6 :
        case 5 :
            rect.top = 4 * level::PIXELS_PER_CELL;
            // ������������� �������� ��� �������� �������
            rect.left = level::base::COLUMNS * level::PIXELS_PER_CELL
                        * (_horizontalFrameNumber - 1);
            break;

        case 4 :
        case 3 :
            rect.top = 6 * level::PIXELS_PER_CELL;
            rect.left = level::base::COLUMNS * level::PIXELS_PER_CELL
                        * (_horizontalFrameNumber - 1);
            break;

        case 2 :
        case 1 :
            rect.top = 8 * level::PIXELS_PER_CELL;
            rect.left = level::base::COLUMNS * level::PIXELS_PER_CELL
                        * (_horizontalFrameNumber - 1);
            break;

        case 0 :
            rect.top = 10 * level::PIXELS_PER_CELL;
            break;

        default :
            break;
    }
    _spriteTerrain->setTextureRect(rect);
}