#pragma once

#include "GameObject.h"


/// <summary>
///  ласс обычной стены
/// <para>Ёкземпл€ры - стены либо с одним очком прочности,
/// либо неу€звимые дл€ урона</para>
/// </summary>
class Wall : public GameObject
{
    public :
        Wall(const class Game& game, sf::IntRect rect,
             bool isInvulnerable = false);
        ~Wall() override {}

        Wall(const Wall&) = delete;
        Wall operator=(const Wall&) = delete;

    protected :
        // ¬ариаци€ спрайта стены (1-3)
        int _variation{0};
};