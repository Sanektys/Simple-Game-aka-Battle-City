#pragma once


/// <summary>
/// Перечисление, представляющее группу, в которую входит игровой объект
/// <para>(это объект окружения, или это объект сущности)</para>
/// </summary>
enum class GameObjectGroup
{
    NONE,

    TERRAIN,
    ENTITY
};

/// <summary>
/// Перечисление существующих типов игровых объектов
/// </summary>
enum class GameObjectType
{
	NONE,

    // Типы игрового окружения
    ///////////////////////////
    WALL = 10,
    STEEL_WALL,
    SOLID_WALL,
    
    // Типы сущностей
    ///////////////////////////
    BASE = 100,
    ENEMY_SPAWNER,
    TANK_ENEMY,
	TANK_FIRST_PLAYER,
	TANK_SECOND_PLAYER,
	BULLET,
};