#pragma once


/// <summary>
/// Перечисление существующих типов игровых объектов
/// </summary>
enum class GameObjectType
{
	NONE,
	TANK_FIRST_PLAYER,
	TANK_SECOND_PLAYER,
	TANK_ENEMY,
	BULLET,
	WALL,
	SOLID_WALL,
	BASE,
	ENEMY_SPAWNER
};