#include "SolidWall.h"
#include "GameObjectType.h"
#include "Utils.h"


SolidWall::SolidWall(const class Game& game, sf::IntRect rect) : Wall(game, rect) {
	setType(GameObjectType::SOLID_WALL);
	_variation = getRandomInt(1, 3);

	setHealth(3);
}

void SolidWall::render(sf::RenderWindow* rw) {
    // Выбор актуального спрайта из атласа
    // по оставшейся прочности стены
	setTextureRect(_spriteTerrain->getTextureRect());
	
	GameObject::render(rw);
}

void SolidWall::setTextureRect(sf::IntRect rect) {
    // Выбор спрайта по очкам прочности стены
    // По умолчанию очков 3, поэтому этого варианта тут нет
	switch (getHealth()) {
	    case 2 :
            // Смещение на одну текстуру вниз в атласе
		    rect.top = 14 * level::PIXELS_PER_CELL;
		    break;

	    case 1 :
		    rect.top = 15 * level::PIXELS_PER_CELL;
		    break;
	}
	_spriteTerrain->setTextureRect(rect);
}