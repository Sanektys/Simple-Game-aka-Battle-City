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
		Wall();
		~Wall();

		/// <summary>
		/// ћетод выбора одного из трЄх типов спрайтов стены из атласа,
        /// в зависимости от вариации стены, сгенерированной конструктором
		/// </summary>
		/// <param name="rect">- пр€моугольник "вырезающий" определЄнный
        /// спрайт из атласа</param>
		virtual void setTextureRect(sf::IntRect rect) override;
    
    private :
        Wall(const Wall&) = delete;
        Wall operator=(const Wall&) = delete;

    protected :
        // ¬ариаци€ спрайта стены (1-3)
        int _variation{0};
};