#pragma once

#include "Level.h"


class Game;


/// <summary>
/// Базовый класс для всех игровых объектов
/// <para>
/// Рассчётные x,y координаты объектов расположены в левом верхнем углу
/// относительно их габаритов
/// </para>
/// </summary>
class GameObject
{
    public :
	    GameObject();

		virtual void render(sf::RenderWindow* rw);
		virtual void update(float dt);

		virtual void intersect(class GameObject* object);
		void escapeSticking();

		void doDamage(int damage);

		// Блок сеттеров/геттеров
		
		enum class GameObjectType getType() { return _type; }

		void setGame(class Game* game) { _game = game; }

		void setX(float x) { _x = x; }
		float getX() { return _x; }

		void setY(float y) { _y = y; }
		float getY() { return _y; }

		void setXSpeed(float xSpeed) { _xSpeed = xSpeed; }
		float getXSpeed() { return _xSpeed; }

		void setYSpeed(float ySpeed) { _ySpeed = ySpeed; }
		float getYSpeed() { return _ySpeed; }

		void setWidth(float width) { _width = width; }
		float getWidth() { return _width; }

		void setHeight(float height) { _height = height; }
		float getHeight() { return _height; }

		void setHealth(int health) { _health = health; }
		int getHealth() { return _health; }

		void setDestroyAfterDeath(bool destroyAfterDeath) { _destroyAfterDeath = destroyAfterDeath; }
		bool getDestroyAfterDeath() { return _destroyAfterDeath; }

		void setInvulnerable(bool invulnerable) { _invulnerable = invulnerable; }
		bool getInvulnerable() { return _invulnerable; }

		void setPhysical(bool physical) { _physical = physical; }
		bool getPhysical() { return _physical; }

		void setDirection(enum class Direction direction) { _direction = direction; }
		enum class Direction getDirection() { return _direction; }

		virtual void setTextureRect(sf::IntRect rect);

		bool getIsSticking() { return _isSticking; }

		void setInBypass(bool inBypass) { _inBypass = inBypass; }
		bool getInBypass() { return _inBypass; }

    protected :
		class Game* _game;
		enum class GameObjectType _type;

		sf::Sprite* _spriteTerrain;
		sf::Sprite* _spriteEntity;

    private :
		float _x;
		float _y;
		float _xSpeed;
		float _ySpeed;

		float _width;
		float _height;

		int _health;
		bool _destroyAfterDeath;
		bool _invulnerable;

		bool _physical;

		enum class Direction _direction;

		bool _isSticking;
		bool _inBypass;
};