#pragma once

#include "Level.h"


class Game;


/// <summary>
/// ������� ����� ��� ���� ������� ��������
/// <para>
/// ��������� x,y ���������� �������� ����������� � ����� ������� ����
/// ������������ �� ���������
/// </para>
/// </summary>
class GameObject
{
    public :
        GameObject() = delete;
        explicit GameObject(const class Game& game);
        virtual ~GameObject() {}

        /// <summary>
        /// ������� ����� ��������� ������� ��������
        /// <para>������������� ������ ������� �� ������� ���������</para>
        /// </summary>
        /// <param name="rw">- ��������� �� ������ ���� ����</param>
        virtual void render(sf::RenderWindow* rw);
        /// <summary>
        /// �������� ����� ���������� ��������� ������� ��������
        /// <para>����������� ����������� ������ ������� � ������,
        /// � ����� ����������� ������� ������� ������� �� ������
        /// ��� �������� � ���������� ������� �� ���������� ����</para>
        /// </summary>
        /// <param name="dt">- �����, ��������� �� ���������� ����
        /// (����� ��������� ������ � ��������� �� ������� ���)</param>
        virtual void update(float dt);

        /// <summary>
        /// ����� ��������� �������� ������� ��� ������������
        /// � ������ ��������
        /// </summary>
        /// <param name="object">- ������, � ������� ����������
        /// �������� ������, ��������� �����</param>
        virtual void intersect(class GameObject* object) {}

        /// <summary>
        /// ����� �������� ����� ������� ��������
        /// </summary>
        /// <param name="damage">- ����, ������� ������� ������� ������</param>
        void doDamage(int damage);

        // ���� ��������/��������
        /////////////////////////////////////////
        /// <summary>
        /// ����� ���������� ������ �������� �������
        /// <para>(��� ������ �������� ���������,
        /// ��� ��� ������ ������� ��������)</para>
        /// </summary>
        /// <returns>���������� ������, � ������� ��������� ������� ������</returns>
        enum GameObjectGroup getGroup() const { return _group; }
        /// <summary>
        /// ����� ���������� ��� ������� �� ��� ����������
        /// </summary>
        /// <returns>���������� ��� �������� �������</returns>
        enum GameObjectType getType() const { return _type; }

        /// <summary>
        /// ��������� ��������� �������� ������� �� �����������
        /// </summary>
        /// <param name="x">- ���������� �� �����������</param>
        void setX(float x) { _x = x; }
        /// <summary>
        /// ��������� ��������� �������� ������� �� �����������
        /// </summary>
        /// <returns>���������� ���������� �� ����������� x �������</returns>
        float getX() const { return _x; }

        /// <summary>
        /// ��������� ��������� �������� ������� �� ���������
        /// </summary>
        /// <param name="y">- ���������� �� ���������</param>
        void setY(float y) { _y = y; }
        /// <summary>
        /// ��������� ��������� �������� ������� �� ���������
        /// </summary>
        /// <returns>���������� ���������� �� ��������� y �������</returns>
        float getY() const { return _y; }

        /// <summary>
        /// ��������� �������������� �������� �������� �������
        /// <para>������������� �������� - �������� ������,
        /// � ������������� - �����</para>
        /// </summary>
        /// <param name="xSpeed">- �������������� ��������</param>
        void setXSpeed(float xSpeed) { _xSpeed = xSpeed; }
        /// <summary>
        /// ��������� �������������� �������� �������� �������
        /// </summary>
        /// <returns>���������� �������������� �������� �������</returns>
        float getXSpeed() const { return _xSpeed; }

        /// <summary>
        /// ��������� ������������ �������� �������� �������
        /// <para>������������� �������� - �������� ����,
        /// � ������������� - �����</para>
        /// </summary>
        /// <param name="ySpeed">- ������������ �������� �������</param>
        void setYSpeed(float ySpeed) { _ySpeed = ySpeed; }
        /// <summary>
        /// ��������� ������������ �������� �������� �������
        /// </summary>
        /// <returns>���������� ������������ �������� �������</returns>
        float getYSpeed() const { return _ySpeed; }

        /// <summary>
        /// ��������� ������ �������� �������
        /// </summary>
        /// <returns>���������� ������ �������</returns>
        float getWidth() const { return _width; }

        /// <summary>
        /// ��������� ������ �������� �������
        /// </summary>
        /// <returns>���������� ������ �������</returns>
        float getHeight() const { return _height; }

        /// <summary>
        /// ��������� ����� ���������(��������) �������� �������
        /// </summary>
        /// <returns>���������� ���� ���������</returns>
        int getHealth() const { return _health; }

        /// <summary>
        /// ��������� �������� ����������, ������������ ��������� �� ���������
        /// �������� ������� ��� ������� ��� ����� ��������� �� ����
        /// </summary>
        /// <returns>���������� true ���� ������ ��������� ��� ����������� �
        /// false ���� ��������� ������� � ������� ��������</returns>
        bool getDestroyAfterDeath() const { return _destroyAfterDeath; }

        /// <summary>
        /// ��������� �������� ����������, ������������
        /// �������� �� ������� ������ ����������
        /// </summary>
        /// <returns>���������� true ���� ������ ��������,
        /// false ���� ������ � �����</returns>
        bool getInvulnerable() const { return _invulnerable; }

        /// <summary>
        /// ��������� �������� ����������, ������������
        /// �������� �� ������� ������ ��������������� � ��������
        /// </summary>
        /// <returns>���������� true ���� ������ ����� ��������,
        /// false ���� ������ �� ����� ��������</returns>
        bool getPhysical() const { return _physical; }

        /// <summary>
        /// ��������� ����������� �������� �������� �������
        /// </summary>
        /// <param name="direction">- ����������� ����������/�������� �������</param>
        virtual void setDirection(enum Direction direction) { _direction = direction; }
        /// <summary>
        /// ��������� ����������� �������� �������� �������
        /// </summary>
        /// <returns>���������� ������� ����������� ��������/���������� �������</returns>
        enum Direction getDirection() const { return _direction; }

        /// <summary>
        /// ��������� �������� ����������,
        /// ������������ ������� �� ������� ������ � ������ �������
        /// </summary>
        /// <returns>���������� true ���� ������ �������, false ���� ���</returns>
        bool getIsSticking() const { return _isSticking; }

        /// <summary>
        /// ��������� �������� ����������, ������������
        /// �������� �� ������� ������ ��������� ������ ������ ������
        /// </summary>
        /// <returns>���������� true ���� ������ �������� ������ ������ ������,
        /// false ���� ���</returns>
        bool getInBypass() const { return _inBypass; }

    protected :
        /// <summary>
        /// ����� ��������� ��������� �������� �� ������
        /// </summary>
        /// <param name="rect">- ������������� "����������"
        /// ������ ������ �� ������</param>
        virtual void setTextureRect(sf::IntRect rect) {}

        /// <summary>
        /// ��������� ������, � ������� ��������� ������� ������
        /// </summary>
        /// <param name="group">- � ����� ������ ��������� ������:
        /// ������� ��������� ��� ������� ��������</param>
        void setGroup(enum GameObjectGroup group) { _group = group; }
        /// <summary>
        /// ��������� ���� �������� �������
        /// </summary>
        /// <param name="type">- ��� �������� �������</param>
        void setType(enum GameObjectType type) { _type = type; }

        /// <summary>
        /// ��������� ������ �������� �������
        /// </summary>
        /// <param name="width">- ������ �������</param>
        void setWidth(float width) { _width = width; }

        /// <summary>
        /// ��������� ������ �������� �������
        /// </summary>
        /// <param name="height">- ������ �������</param>
        void setHeight(float height) { _height = height; }

        /// <summary>
        /// ��������� ����� ���������(��������) �������� �������
        /// </summary>
        /// <param name="health">- ���� ��������� �������</param>
        void setHealth(int health) { _health = health; }

        /// <summary>
        /// ��������� ����� �������� ���������� �������� �������
        /// ����� ������� ��� ����� ��������� �� ����
        /// </summary>
        /// <param name="destroyAfterDeath">- ������� �� ������
        /// ����� �����������</param>
        void setDestroyAfterDeath(bool destroyAfterDeath) {
            _destroyAfterDeath = destroyAfterDeath;
        }

        /// <summary>
        /// ��������� ��������� ������������ �������� ������� � �����
        /// </summary>
        /// <param name="invulnerable">- �������� �� ������ ����������</param>
        void setInvulnerable(bool invulnerable) { _invulnerable = invulnerable; }

        /// <summary>
        /// ��������� ��������� ����������������� �������� ������� � ��������
        /// </summary>
        /// <param name="physical">- ���� �� � ������� ��������</param>
        void setPhysical(bool physical) { _physical = physical; }

        /// <summary>
        /// ��������� ��������� �������� �������,
        /// ����� �� ��������� �������� ������ ������ ������
        /// </summary>
        /// <param name="inBypass">- �������� �� ������
        /// ��������� ������ ������ ������</param>
        void setInBypass(bool inBypass) { _inBypass = inBypass; }

        /// <summary>
        /// ��������� �� ������ ������� ����, � ����� ������������� ��� �������
        /// </summary>
        /// <returns>���������� ������ �� ����������� ������ ������� ������</returns>
        const class Game& getGame() const { return _game; }

        GameObject(const GameObject&) = delete;
        GameObject operator=(const GameObject&) = delete;

    protected:
        // ������ ������ ���������� ������ ���� �� ���� �������������� ��������
        // � ����������� �� ������ ����
        ////////////////
        std::unique_ptr<sf::Sprite> _spriteTerrain; // ��������� �� ������ ���������
        std::unique_ptr<sf::Sprite> _spriteEntity; // ��������� �� ������ ��������

    private :
        /// <summary>
        /// ����� ��������� ����������� �������� ������� � ������ �������
        /// </summary>
        void escapeSticking();

    private :
        // ������ �� ����� ������� ������ ��� ������ ��� �������
        const class Game& _game;
        // ����� �������� �������
        enum GameObjectGroup _group;
        // ��� �������� �������
        enum GameObjectType _type;

        // ���������� �������� �������
        ////////////////
        float _x{0.0f}; // ������� �� �����������
        float _y{0.0f}; // ������� �� ���������
        // �������� �������� ������� �� ����
        ////////////////
        float _xSpeed{0.0f}; // �������� �� �����������
        float _ySpeed{0.0f}; // �������� �� ���������

        // �������� �������� �������
        ////////////////
        float _width{1.0f};  // ������ �������
        float _height{1.0f}; // ������ �������

        // ������� ����������� �������� �������
        enum Direction _direction;

        // ���� ��������� ������� ��� ������������
        ////////////////
        int _health{1}; // �������� (���� ���������) �������� �������
        bool _destroyAfterDeath{true}; // ������� ��� ��� ������ ����� �����������
        bool _invulnerable{false}; // �������� �� ������ ����������

        // ��������� �� ������ �������� � ������� ���������
        bool _physical{true};

        // ��������� ������� ��� ����������� � ������ ��������
        ////////////////
        bool _isSticking{false}; // ������� ��� ��� ������ � ������ �������
        bool _inBypass{false}; // �������� �� ��������� ������ ������ ������ ������
};
