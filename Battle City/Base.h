#pragma once

#include "GameObject.h"


class Base : public GameObject
{
    public :
	    Base();
		~Base() override;

	    virtual void render(sf::RenderWindow* rw) override;
		virtual void update(float dt) override;

		virtual void setTextureRect(sf::IntRect rect) override;

	private :
	    // Переменные для смены спрайта базы
	    float _frameSwitchTime{0.0f};
	    short _horizontalFrameNumber{1};
		bool _toLeft{true};
};