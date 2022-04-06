#include "CoordPoint.h"
#include "GameObject.h"


void CoordPoint::setCoordPoint(class GameObject* modifyObject,
	                           enum typeCoordPoint typePoint) {
	width  = modifyObject->getWidth() / 2.0f - 0.1f;
	height = modifyObject->getHeight() / 2.0f - 0.1f;
	
	switch (typePoint) {
	    case typeCoordPoint::TOP_LEFT : {
			coordY = modifyObject->getY();
			coordX = modifyObject->getX();
			break;
	    }
		case typeCoordPoint::TOP_RIGHT : {
			coordY = modifyObject->getY();
			coordX = modifyObject->getX() + modifyObject->getWidth() - width;
			break;
		}
		case typeCoordPoint::BOTTOM_RIGHT : {
			coordY = modifyObject->getY() + modifyObject->getHeight() - height;
			coordX = modifyObject->getX() + modifyObject->getWidth() - width;
			break;
		}
		case typeCoordPoint::BOTTOM_LEFT : {
			coordY = modifyObject->getY() + modifyObject->getHeight() - height;
			coordX = modifyObject->getX();
			break;
		}
	}	
}