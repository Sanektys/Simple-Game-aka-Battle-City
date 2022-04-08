#include "CoordPoint.h"
#include "GameObject.h"


void CoordPoint::setCoordPoint(class GameObject* modifyObject,
                               enum TypeCoordPoint typePoint) {
    width  = modifyObject->getWidth() / 2.0f - 0.1f;
    height = modifyObject->getHeight() / 2.0f - 0.1f;

    switch (typePoint) {
        case TypeCoordPoint::TOP_LEFT : {
            coordY = modifyObject->getY();
            coordX = modifyObject->getX();
            break;
        }
        case TypeCoordPoint::TOP_RIGHT : {
            coordY = modifyObject->getY();
            coordX = modifyObject->getX() + modifyObject->getWidth() - width;
            break;
        }
        case TypeCoordPoint::BOTTOM_RIGHT : {
            coordY = modifyObject->getY() + modifyObject->getHeight() - height;
            coordX = modifyObject->getX() + modifyObject->getWidth() - width;
            break;
        }
        case TypeCoordPoint::BOTTOM_LEFT : {
            coordY = modifyObject->getY() + modifyObject->getHeight() - height;
            coordX = modifyObject->getX();
            break;
        }
    }
}