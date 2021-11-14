#pragma once

#include "Wall.h"


class SolidWall : public Wall
{
    public :
        SolidWall();

        virtual void render(sf::RenderWindow* rw) override;
        virtual void setTextureRect(sf::IntRect rect) override;
};