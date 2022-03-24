#pragma once

#include <SFML/Graphics.hpp>
#include <Windows.h>

#include "Direction.h"
#include "GameObjectType.h"


extern sf::Texture* _atlasTerrain;
extern sf::Texture* _atlasEntity;

const short SCREEN_ROWS     = 30;
const short SCREEN_COLUMNS  = 60;
const short LEVEL_ROWS      = SCREEN_ROWS;
const short LEVEL_COLUMNS   = 50;
const short PIXELS_PER_CELL = 32;

const unsigned char CELL_SYMBOL_PLAYER_1      = '1';
const unsigned char CELL_SYMBOL_PLAYER_2      = '2';
const unsigned char CELL_SYMBOL_TANK          = 'T';
const unsigned char CELL_SYMBOL_BRICK_WALL    = '#';
const unsigned char CELL_SYMBOL_SOLID_WALL    = '%';
const unsigned char CELL_SYMBOL_STEEL_WALL    = '@';
const unsigned char CELL_SYMBOL_BASE          = 'B';
const unsigned char CELL_SYMBOL_ENEMY_SPAWNER = 'S';

const unsigned char FIRST_LEVEL_DATA[LEVEL_ROWS][LEVEL_COLUMNS + 1] =
{
	"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
	"@S      ##                              ##     S @",
	"@       ##                              ##       @",
	"@  %%   ##  #%%%%%#            #%%%%%#  ##   %%  @",
	"@%%%@@@@##@@@@###@@@@##    ##@@@@###@@@@##@@@@%%%@",
	"@%%   @@@@@@       @@@@@##@@@@@       @@@@@@   %%@",
	"@       @@            @@@@@@            @@       @",
	"@                       @@                       @",
	"@                       @@                       @",
	"@   @@@######     @@@        @@@     ######@@@   @",
	"@   @@@######     @@@   S    @@@     ######@@@   @",
	"@      ##         @@@        @@@         ##      @",
	"@                 @@@@@@@@@@@@@@                 @",
	"@                      @@@@                      @",
	"@      @@@              @@              @@@      @",
	"@      @@@              @@              @@@      @",
	"@###   #%%#########            ########%%##   ###@",
	"@%%#   ##%%%%@@%%####   S    ####%%@@%%%%##   #%%@",
	"@###    ###%%@@@%%###        ###%%@@@%%###    ###@",
	"@         ###@@@@@%%%@@    @@%%%@@@@@###         @",
	"@           #######%%@@@@@@@@%%#######           @",
	"@                 ##%%######%%##                 @",
	"@   ##               ########               ##   @",
	"@   %%#                                    #%%   @",
	"@   @@@######                        ######@@@   @",
	"@   @@@%%####                        ####%%@@@   @",
	"@      %%       1   ##########  2        %%      @",
	"@      ##          ##B       ##          ##      @",
	"@                  @@        @@                  @",
	"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
};

const sf::IntRect BRICK_WALL_IMAGE       = sf::IntRect(0, 384, PIXELS_PER_CELL, PIXELS_PER_CELL);
const sf::IntRect SOLID_BRICK_WALL_IMAGE = sf::IntRect(0, 416, PIXELS_PER_CELL, PIXELS_PER_CELL);
const sf::IntRect STEEL_WALL_IMAGE       = sf::IntRect(0, 512, PIXELS_PER_CELL, PIXELS_PER_CELL);

/////////////////////////////////////
// Информация о танках
const float TANK_FIRE_COOLDOWN_TIME = 1.8f;
const float TANK_WIDTH  = 2.0f;
const float TANK_HEIGHT = 2.25f;

const sf::IntRect FIRST_PLAYER_TANK_IMAGE  = sf::IntRect(0, 0, 64, 88);
const sf::IntRect SECOND_PLAYER_TANK_IMAGE = sf::IntRect(0, 88, 64, 88);
const sf::IntRect BASIC_ENEMY_TANK_IMAGE   = sf::IntRect(0, 176, 64, 88);

////////////////////////////////////
// Информация об игроке
const short PLAYER_HEALTH = 16;
const float PLAYER_SPEED  = 6.0f;

////////////////////////////////////
// Информация о противниках
const short ENEMIES_PER_LEVEL = 9;
const short ENEMIES_PER_LEVEL_IN_ONE_MOMENT = 5;
const short ENEMY_HEALTH = 6;
const float ENEMY_SPEED  = 4.5f;
const float ENEMY_SPAWNER_SPAWN_TIME = 10.0f;
const float ENEMY_AI_ANALIZER_TIME   = 0.8f;

////////////////////////////////////
// Информация о базе
const int BASE_COLUMNS   = 8;
const int BASE_ROWS      = 2;
const short BASE_HEALTH  = 10;

const sf::IntRect BASE_IMAGE = sf::IntRect(0, 0, PIXELS_PER_CELL * BASE_COLUMNS,
										         PIXELS_PER_CELL * BASE_ROWS);

/////////////////////////////////////
// Информация о снарядах
const float BULLET_SPEED = 20.0f;
const float STANDART_BULLET_WIDTH  = 0.125f;
const float STANDART_BULLET_HEIGHT = 0.25f;

const sf::IntRect BULLET_IMAGE  = sf::IntRect(0, 264, 6, 10);