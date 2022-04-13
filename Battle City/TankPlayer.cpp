#include "TankPlayer.h"


TankPlayer::TankPlayer(const class Game& game, sf::IntRect rect,
                       sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight,
                       sf::Keyboard::Key keyUp,   sf::Keyboard::Key keyDown,
                       sf::Keyboard::Key keyFire)
    : Tank(game), _keyLeft(keyLeft), _keyRight(keyRight),
      _keyUp(keyUp), _keyDown(keyDown), _keyFire(keyFire) {
    setType(GameObjectType::TANK_FIRST_PLAYER);

    setHealth(level::tank::player::HEALTH);
    setMaxSpeed(level::tank::player::SPEED);

    setSpeedup(level::tank::player::SPEED);
    setBrakingSpeed(level::tank::player::SPEED);
    setRotationTime(0.6f);

    setDestroyAfterDeath(false);

    _spriteEntity->setTextureRect(rect);
}

void TankPlayer::update(float dt) {
    if (sf::Keyboard::isKeyPressed(_keyFire)
        || sf::Keyboard::isKeyPressed(_altKeyFire))
        fire();

    if (sf::Keyboard::isKeyPressed(_keyLeft)
        || sf::Keyboard::isKeyPressed(_altKeyLeft))
        move(Direction::LEFT, dt);

    else if (sf::Keyboard::isKeyPressed(_keyRight)
             || sf::Keyboard::isKeyPressed(_altKeyRight))
        move(Direction::RIGHT, dt);

    else if (sf::Keyboard::isKeyPressed(_keyUp)
             || sf::Keyboard::isKeyPressed(_altKeyUp))
        move(Direction::UP, dt);

    else if (sf::Keyboard::isKeyPressed(_keyDown)
             || sf::Keyboard::isKeyPressed(_altKeyDown))
        move(Direction::DOWN, dt);

    else
        move(Direction::NONE, dt);

    Tank::update(dt);
}

void  TankPlayer::setKeys(sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight,
                          sf::Keyboard::Key keyUp,   sf::Keyboard::Key keyDown,
                          sf::Keyboard::Key keyFire) {
    _keyLeft  = keyLeft;
    _keyRight = keyRight;
    _keyUp    = keyUp;
    _keyDown  = keyDown;
    _keyFire  = keyFire;
}

void TankPlayer::setAltKeys(sf::Keyboard::Key keyLeft, sf::Keyboard::Key keyRight,
                            sf::Keyboard::Key keyUp,   sf::Keyboard::Key keyDown,
                            sf::Keyboard::Key keyFire) {
    _altKeyLeft  = keyLeft;
    _altKeyRight = keyRight;
    _altKeyUp    = keyUp;
    _altKeyDown  = keyDown;
    _altKeyFire  = keyFire;
}