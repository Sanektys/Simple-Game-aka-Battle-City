#include "SolidWall.h"
#include "GameObjectType.h"
#include "Utils.h"


SolidWall::SolidWall(const class Game& game, sf::IntRect rect) : Wall(game, rect) {
    setType(GameObjectType::SOLID_WALL);
    _variation = getRandomInt(1, 3);

    setHealth(3);
}

void SolidWall::render(sf::RenderWindow* rw) {
    // ����� ����������� ������� �� ������
    // �� ���������� ��������� �����
    setTextureRect(_spriteTerrain->getTextureRect());

    GameObject::render(rw);
}

void SolidWall::setMapPin() {
    _rectanglePinOnMap.setSize(sf::Vector2f(getWidth()  * level::PIXELS_PER_CELL,
                                            getHeight() * level::PIXELS_PER_CELL));
    _rectanglePinOnMap.setPosition(getX() * level::PIXELS_PER_CELL,
                                   getY() * level::PIXELS_PER_CELL);
    _rectanglePinOnMap.setFillColor(sf::Color(119, 36, 18));
}

void SolidWall::setTextureRect(sf::IntRect rect) {
    // ����� ������� �� ����� ��������� �����
    // �� ��������� ����� 3, ������� ����� �������� ��� ���
    switch (getHealth()) {
        case 2:
            // �������� �� ���� �������� ���� � ������
            rect.top = 14 * level::PIXELS_PER_CELL;
            break;

        case 1:
            rect.top = 15 * level::PIXELS_PER_CELL;
            break;

        default:
            break;
    }

    _spriteTerrain->setTextureRect(rect);
}