#include "Wall.h"
#include "Utils.h"
#include "Level.h"
#include "GameObjectType.h"


Wall::Wall(const class Game& game, sf::IntRect rect, bool isInvulnerable)
    : GameObject(game) {
    setInvulnerable(isInvulnerable);

    setGroup(GameObjectGroup::TERRAIN);
    if (isInvulnerable)
        setType(GameObjectType::STEEL_WALL);
    else
        setType(GameObjectType::WALL);

    _variation = getRandomInt(1, 3);

    _spriteTerrain.reset(new sf::Sprite());
    _spriteTerrain->setTexture(*ATLAS_TERRAIN);
    _spriteTerrain->setOrigin((getWidth() * (float)PIXELS_PER_CELL) / 2.0f,
                              (getHeight() * (float)PIXELS_PER_CELL) / 2.0f);

    // ����� �������� ������� ��� ����������� � ����������� �� ���������� ����
    // �� ��������� ��� �������� ������ ��� 1, ������� ��� ��� ���
    switch (_variation) {
        case 2:
            rect.left = PIXELS_PER_CELL;
            break;

        case 3:
            rect.left = 2 * PIXELS_PER_CELL;
            break;
    }
    _spriteTerrain->setTextureRect(rect);
}