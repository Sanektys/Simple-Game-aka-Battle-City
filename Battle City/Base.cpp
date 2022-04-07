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

void Base::update(float dt) {
    // Задержка на переключение спрайта базы
    _frameSwitchTime += dt;
    if (0.25f <= _frameSwitchTime)
        _frameSwitchTime = 0.0f;   // Когда ноль - сменить спрайт
}

void Base::setTextureRect(sf::IntRect rect) {
    // Предварительное смещение по горизонтали в атласе для анимации
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
    // Переключение спрайтов (по вертикали) по уровню оставшейся прочности
    switch (getHealth()) {
        case 8 :
        case 7 :
            rect.top = 2 * level::PIXELS_PER_CELL;
            break;

        case 6 :
        case 5 :
            rect.top = 4 * level::PIXELS_PER_CELL;
            // Использование смещения для анимации горения
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