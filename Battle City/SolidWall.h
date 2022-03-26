#pragma once

#include "Wall.h"


/// <summary>
///  ласс прочной стены
/// <para>Ёкземпл€ры класса отличаютс€ увеличенными очками прочности</para>
/// </summary>
class SolidWall : public Wall
{
    public :
        SolidWall();
        ~SolidWall() override {}

        /// <summary>
        /// ћетод отрисовки стены
        /// </summary>
        /// <param name="rw">- указатель на объект окна игры</param>
        virtual void render(sf::RenderWindow* rw) override;
        /// <summary>
        /// ћетод выбора актуального, по очкам прочности стены, спрайта
        /// </summary>
        /// <param name="rect">- пр€моугольник "вырезающий" нужный
        /// спрайт из атласа</param>
        virtual void setTextureRect(sf::IntRect rect) override;

    private :
        SolidWall(const SolidWall&) = delete;
        SolidWall operator=(const SolidWall&) = delete;
};