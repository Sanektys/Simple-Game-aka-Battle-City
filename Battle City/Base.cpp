#include "Base.h"
#include "GameObjectType.h"
#include "Level.h"


Base::Base() {
	_type = GameObjectType::BASE;

	setWidth(BASE_COLUMNS);
	setHeight(BASE_ROWS);

	setHealth(BASE_HEALTH);
	setDestroyAfterDeath(false);

	_spriteTerrain = new sf::Sprite();
	_spriteTerrain->setTexture(*_atlasTerrain);
	_spriteTerrain->setOrigin(getWidth()  * PIXELS_PER_CELL / 2.0f,
							  getHeight() * PIXELS_PER_CELL / 2.0f);
}

Base::~Base() {
	if (_spriteTerrain) {
		delete _spriteTerrain;
		_spriteTerrain = nullptr;
	}
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
			rect.top = 2 * PIXELS_PER_CELL;
			break;

		case 6 :
		case 5 :
			rect.top = 4 * PIXELS_PER_CELL;
			// Использование смещения для анимации горения
			rect.left = BASE_COLUMNS * PIXELS_PER_CELL
					    * (_horizontalFrameNumber - 1);
			break;

		case 4 :
		case 3 :
			rect.top = 6 * PIXELS_PER_CELL;
			rect.left = BASE_COLUMNS * PIXELS_PER_CELL
				        * (_horizontalFrameNumber - 1);
			break;

		case 2 :
		case 1 :
			rect.top = 8 * PIXELS_PER_CELL;
			rect.left = BASE_COLUMNS * PIXELS_PER_CELL
				        * (_horizontalFrameNumber - 1);
			break;

		case 0 :
			rect.top = 10 * PIXELS_PER_CELL;
			break;
	}   
	_spriteTerrain->setTextureRect(rect);
}