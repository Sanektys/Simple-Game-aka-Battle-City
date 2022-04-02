#pragma once


/// <summary>
/// ������������, �������������� ������, � ������� ������ ������� ������
/// <para>(��� ������ ���������, ��� ��� ������ ��������)</para>
/// </summary>
enum class GameObjectGroup
{
    NONE,

    TERRAIN,
    ENTITY
};

/// <summary>
/// ������������ ������������ ����� ������� ��������
/// </summary>
enum class GameObjectType
{
	NONE,

    // ���� �������� ���������
    ///////////////////////////
    WALL = 10,
    STEEL_WALL,
    SOLID_WALL,
    
    // ���� ���������
    ///////////////////////////
    BASE = 100,
    ENEMY_SPAWNER,
    TANK_ENEMY,
	TANK_FIRST_PLAYER,
	TANK_SECOND_PLAYER,
	BULLET,
};