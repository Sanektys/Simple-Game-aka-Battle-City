#pragma once

#include "GameObject.h"



class Wall : public GameObject
{
    public :
		Wall();
		~Wall();

		virtual void render(sf::RenderWindow* rw) override;
		virtual void setTextureRect(sf::IntRect rect) override;

    protected :
		int _variation;
};