#pragma once

#include "GameObject.h"
#include "Tank.h"


class Bullet : public GameObject
{
    public :
        friend class Tank;

        Bullet(const class Game& game, sf::IntRect rect,
               enum Direction direction = Direction(0),
               float speedX = 0.0f, float speedY = 0.0f,
               enum GameObjectType onwer = GameObjectType(0));
        ~Bullet() override {}

        /// <summary>
        /// Метод, определяющий поведение объекта снаряда при коллизии
        /// с другим объектом
        /// </summary>
        /// <param name="object">- объект,
        /// с которым произошло столкновение/пересечение</param>
        void intersect(class GameObject* object) override;

        /// <summary>
        /// Получение типа объекта, который выпустил данный снаряд
        /// </summary>
        /// <returns>Возвращает тип объект-хозаина
        ///  данного экземпляра снаряда</returns>
        enum GameObjectType getOwnerType() const { return _ownerType; }

        Bullet(const Bullet&) = delete;
        Bullet operator=(const Bullet&) = delete;

    private :
        /// <summary>
        /// Установка спрайта снаряда согласно типу хозяина и
        /// поворот согласно направлению полёта
        /// </summary>
        /// <param name="rect">- прямоугольник, "вырезающий" нужный спрайт
        /// из атласа текстуры</param>
        void setTextureRect(sf::IntRect rect) override;

        /// <summary>
        /// Установка направления полёта снаряда
        /// и инверсия его изначальных габаритов если он летит по горизонтали
        /// </summary>
        /// <param name="direction">- направление полёта снаряда</param>
        void setDirection(enum Direction direction) override;

        /// <summary>
        /// Установка типа объекта, который выпустил данный снаряд
        /// </summary>
        /// <param name="owner">- тип объекта-хозяина данного
        /// экземпляра снаряда</param>
        void setOwnerType(enum GameObjectType owner) { _ownerType = owner; }

    private :
        // Тип объекта, что выпустил данный снаряд
        enum GameObjectType _ownerType;
};