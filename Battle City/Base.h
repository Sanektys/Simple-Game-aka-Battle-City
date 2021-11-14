#pragma once

#include "GameObject.h"


class Base : public GameObject
{
    public :
	    Base();
		~Base();

	    virtual void render(sf::RenderWindow* rw) override;
		virtual void setTextureRect(sf::IntRect rect) override;
};