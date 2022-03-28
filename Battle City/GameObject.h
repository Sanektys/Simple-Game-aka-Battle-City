#pragma once

#include "level.h"


class Game;


/// <summary>
/// Базовый класс для всех игровых объектов
/// <para>
/// Расчётные x,y координаты объектов расположены в левом верхнем углу
/// относительно их габаритов
/// </para>
/// </summary>
class GameObject
{
    public :
        GameObject();
        virtual ~GameObject() {}

        /// <summary>
        /// Базовый метод отрисовки игровых объектов
        /// <para>Позиционирует спрайт объекта на игровой плоскости</para>
        /// </summary>
        /// <param name="rw">- указатель на объект окна игры</param>
        virtual void render(sf::RenderWindow* rw);
        /// <summary>
        /// Основной метод обновления состояния игровых объектов
        /// <para>Проверяется застревание одного объекта в другом,
        /// а также вычисляется текущая позиция объекта на основе
        /// его скорости и прошедшего времени за предыдущий такт</para>
        /// </summary>
        /// <param name="dt">- время, прошедшее за предыдущий такт
        /// (время обработки логики и отрисовки за прошлый раз)</param>
        virtual void update(float dt);

        /// <summary>
        /// Метод поведения игрового объекта при столкновении
        /// с другим объектом
        /// </summary>
        /// <param name="object">- объект, с которым столкнулся
        /// исходный объект, вызвавший метод</param>
        virtual void intersect(class GameObject* object) {}
        /// <summary>
        /// Метод избежания застревания игрового объекта в другом объекте
        /// </summary>
        void escapeSticking();

        /// <summary>
        /// Метод принятия урона игровым объектом
        /// </summary>
        /// <param name="damage">- урон, который получил игровой объект</param>
        void doDamage(int damage);

        // Блок сеттеров/геттеров
        /////////////////////////////////////////       
        /// <summary>
        /// Метод извлекает требуемую текстуру из атласа
        /// </summary>
        /// <param name="rect">- прямоугольник "вырезающий"
        /// нужный спрайт из атласа</param>
        virtual void setTextureRect(sf::IntRect rect) {}
        
        /// <summary>
        /// Метод возвращает тип объекта из его экземпляра
        /// </summary>
        /// <returns>Возвращает тип игрового объекта</returns>
        enum class GameObjectType getType() const { return _type; }

        /// <summary>
        /// Связывает игровой объект с общим объектом игровой логики,
        /// путём сохранения указателя на последний, с целью дальнейшего
        /// использования его методов
        /// </summary>
        /// <param name="game">- указатель на общий для всех
        /// объект игровой логики</param>
        void setGame(class Game* game) { _game = game; }

        /// <summary>
        /// Установка положения игрового объекта по горизонтали
        /// </summary>
        /// <param name="x">- координата по горизонтали</param>
        void setX(float x) { _x = x; }
        /// <summary>
        /// Получение положения игрового объекта по горизонтали
        /// </summary>
        /// <returns>Возвращает координату по горизонтали x объекта</returns>
        float getX() const { return _x; }

        /// <summary>
        /// Установка положения игрового объекта по вертикали
        /// </summary>
        /// <param name="y">- координата по вертикали</param>
        void setY(float y) { _y = y; }
        /// <summary>
        /// Получение положения игрового объекта по вертикали
        /// </summary>
        /// <returns>Возвращает координату по вертикали y объекта</returns>
        float getY() const { return _y; }

        /// <summary>
        /// Установка горизонтальной скорости игрового объекта
        /// <para>Положительная скорость - движение вправо,
        /// а отрицательная - влево</para>
        /// </summary>
        /// <param name="xSpeed">- горизонтальная скорость</param>
        void setXSpeed(float xSpeed) { _xSpeed = xSpeed; }
        /// <summary>
        /// Получение горизонтальной скорости игрового объекта
        /// </summary>
        /// <returns>Возвращает горизонтальную скорость объекта</returns>
        float getXSpeed() const { return _xSpeed; }

        /// <summary>
        /// Установка вертикальной скорости игрового объекта
        /// <para>Положительная скорость - движение вниз,
        /// а отрицательная - вверх</para>
        /// </summary>
        /// <param name="ySpeed">- вертикальная скорость объекта</param>
        void setYSpeed(float ySpeed) { _ySpeed = ySpeed; }
        /// <summary>
        /// Получение вертикальной скорости игрового объекта
        /// </summary>
        /// <returns>Возвращает вертикальную скорость объекта</returns>
        float getYSpeed() const { return _ySpeed; }

        /// <summary>
        /// Установка ширины игрового объекта
        /// </summary>
        /// <param name="width">- ширина объекта</param>
        void setWidth(float width) { _width = width; }
        /// <summary>
        /// Получение ширины игрового объекта
        /// </summary>
        /// <returns>Возвращает ширину объекта</returns>
        float getWidth() const { return _width; }

        /// <summary>
        /// Установка высоты игрового объекта
        /// </summary>
        /// <param name="height">- высота объекта</param>
        void setHeight(float height) { _height = height; }
        /// <summary>
        /// Получение высоты игрового объекта
        /// </summary>
        /// <returns>Возвращает высоту объекта</returns>
        float getHeight() const { return _height; }

        /// <summary>
        /// Установка очков прочности(здоровья) игрового объекта
        /// </summary>
        /// <param name="health">- очки прочности объекта</param>
        void setHealth(int health) { _health = health; }
        /// <summary>
        /// Получение очков прочности(здоровья) игрового объекта
        /// </summary>
        /// <returns>Возвращает очки прочности</returns>
        int getHealth() const { return _health; }

        /// <summary>
        /// Установка права удаления экземпляра игрового объекта
        /// после падения его очков прочности до нуля
        /// </summary>
        /// <param name="destroyAfterDeath">- удалять ли объект
        /// после уничтожения</param>
        void setDestroyAfterDeath(bool destroyAfterDeath)
            { _destroyAfterDeath = destroyAfterDeath; }
        /// <summary>
        /// Получение условной переменной, определяющей удаляется ли экземпляр
        /// игрового объекта при падении его очков прочности до нуля
        /// </summary>
        /// <returns>Возвращает true если объект удаляется при уничтожении и
        /// false если экземпляр остаётся в массиве объектов</returns>
        bool getDestroyAfterDeath() const { return _destroyAfterDeath; }

        /// <summary>
        /// Установка состояния неуязвимости игрового объекта к урону
        /// </summary>
        /// <param name="invulnerable">- является ли объект неуязвимым</param>
        void setInvulnerable(bool invulnerable) { _invulnerable = invulnerable; }
        /// <summary>
        /// Получение условной переменной, определяющей
        /// является ли игровой объект неуязвимым
        /// </summary>
        /// <returns>Возвращает true если объект неуязвим,
        /// false если уязвим к урону</returns>
        bool getInvulnerable() const { return _invulnerable; }

        /// <summary>
        /// Установка состояния невосприимчивости игрового объекта к коллизии
        /// </summary>
        /// <param name="physical">- есть ли у объекта коллизия</param>
        void setPhysical(bool physical) { _physical = physical; }
        /// <summary>
        /// Получение условной переменной, определяющей
        /// является ли игровой объект невосприимчивым к коллизии
        /// </summary>
        /// <returns>Возвращает true если объект имеет коллизию,
        /// false если объект не имеет коллизии</returns>
        bool getPhysical() const { return _physical; }

        /// <summary>
        /// Установка направления движения игрового объекта
        /// </summary>
        /// <param name="direction">- направление ориентации/движения объекта</param>
        void setDirection(enum class Direction direction) { _direction = direction; }
        /// <summary>
        /// Получение направления движения игрового объекта
        /// </summary>
        /// <returns>Возвращает текущее направление движения/ориентации объекта</returns>
        enum class Direction getDirection() const { return _direction; }

        /// <summary>
        /// Получение условной переменной,
        /// определяющей застрял ли игровой объект в другом объекте
        /// </summary>
        /// <returns>Возвращает true если объект застрял, false если нет</returns>
        bool getIsSticking() const { return _isSticking; }

        /// <summary>
        /// Установка состояния игрового объекта,
        /// когда он намеренно проходит сквозь другой объект
        /// </summary>
        /// <param name="inBypass">- проходит ли объект
        /// намеренно сквозь другой объект</param>
        void setInBypass(bool inBypass) { _inBypass = inBypass; }
        /// <summary>
        /// Получение условной переменной, определяющей
        /// проходит ли игровой объект намеренно сквозь другой объект
        /// </summary>
        /// <returns>Возвращает true если объект проходит сквозь другой объект,
        /// false если нет</returns>
        bool getInBypass() const { return _inBypass; }

    protected :
        // Указатель на класс игровой логики для вызова его методов
        class Game* _game{nullptr};
        // Тип игрового объекта
        enum class GameObjectType _type;

        // Каждый объект использует только один из двух представленных спрайтов
        // в зависимости от своего типа
        ////////////////
        sf::Sprite* _spriteTerrain{nullptr}; // Указатель на спрайт окружения
        sf::Sprite* _spriteEntity{nullptr}; // Указатель на спрайт сущности

    private :
        GameObject(const GameObject&) = delete;
        GameObject operator=(const GameObject&) = delete;

    private :
        // Координаты игрового объекта
        ////////////////
        float _x{0.0f}; // Позиция по горизонтали
        float _y{0.0f}; // Позиция по вертикали
        // Скорость игрового объекта по осям
        ////////////////
        float _xSpeed{0.0f}; // Скорость по горизонтали
        float _ySpeed{0.0f}; // Скорость по вертикали

        // Габариты игрового объекта
        ////////////////
        float _width{1.0f};  // Ширина объекта
        float _height{1.0f}; // Высота объекта

        // Текущее направление игрового объекта
        enum class Direction _direction;

        // Очки прочности объекта или неуязвимость
        ////////////////
        int _health{1}; // Здоровье (очки прочности) игрового объекта
        bool _destroyAfterDeath{true}; // Удалять или нет объект после уничтожения
        bool _invulnerable{false}; // Является ли объект неуязвимым

        // Подвержен ли объект коллизии с другими объектами
        bool _physical{true};

        // Состояния объекта при застревании в других объектах
        ////////////////
        bool _isSticking{false}; // Застрял или нет объект в другом объекте
        bool _inBypass{false}; // Проходит ли намеренно объект сквозь другой объект
};