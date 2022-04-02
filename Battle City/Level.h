#pragma once

#include <SFML/Graphics.hpp>

#include "Direction.h"
#include "GameObjectType.h"


/// <summary>
/// Игровые константы и карты уровней
/// </summary>
namespace level {
    extern sf::Texture* ATLAS_TERRAIN;
    extern sf::Texture* ATLAS_ENTITY;


    const short ROWS      = 30;
    const short COLUMNS   = 50;
    const short PIXELS_PER_CELL = 32;

    const short CAMERA_WIDTH  = 16 * 60;
    const short CAMERA_HEIGHT = 9 * 60;

    const unsigned char SYMBOL_PLAYER_1      = '1';
    const unsigned char SYMBOL_PLAYER_2      = '2';
    const unsigned char SYMBOL_TANK          = 'T';
    const unsigned char SYMBOL_BRICK_WALL    = '#';
    const unsigned char SYMBOL_SOLID_WALL    = '%';
    const unsigned char SYMBOL_STEEL_WALL    = '@';
    const unsigned char SYMBOL_BASE          = 'B';
    const unsigned char SYMBOL_ENEMY_SPAWNER = 'S';

    const unsigned char FIRST_MAP[ROWS][COLUMNS + 1] =
    {
        "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@",
        "@S      ##                              ##     S @",
        "@       ##                              ##       @",
        "@       ##  #%%%%%#            #%%%%%#  ##       @",
        "@%%%@@@@##@@@@###@@@@##    ##@@@@###@@@@##@@@@%%%@",
        "@%%   @@@@@@       @@@@@##@@@@@       @@@@@@   %%@",
        "@       @@            @@@@@@            @@       @",
        "@                       @@                       @",
        "@                       @@                       @",
        "@   @@@######     @@@   S    @@@     ######@@@   @",
        "@   @@@######     @@@        @@@     ######@@@   @",
        "@      ##         @@@        @@@         ##      @",
        "@                 @@@@@@@@@@@@@@                 @",
        "@                      @@@@                      @",
        "@      @@@              @@              @@@      @",
        "@      @@@              @@              @@@      @",
        "@###   #%%#########     S      ########%%##   ###@",
        "@%%#   ##%%%%@@%%####        ####%%@@%%%%##   #%%@",
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

    /////////////////////////////////////
    // Параметры стен
    namespace wall {
        const sf::IntRect BRICK_IMAGE      (0, 384, PIXELS_PER_CELL, PIXELS_PER_CELL);
        const sf::IntRect SOLID_BRICK_IMAGE(0, 416, PIXELS_PER_CELL, PIXELS_PER_CELL);
        const sf::IntRect STEEL_IMAGE      (0, 512, PIXELS_PER_CELL, PIXELS_PER_CELL);
    }

    /////////////////////////////////////
    // Параметры танков
    namespace tank {
        const short PIXELS_WIDTH  = 62;
        const short PIXELS_HEIGHT = 71;
        const short PIXELS_GUN_LENGTH = 17;

        const float WIDTH  = float(PIXELS_WIDTH) / PIXELS_PER_CELL;
        const float HEIGHT = float(PIXELS_HEIGHT) / PIXELS_PER_CELL;
        const float GUN_LENGTH = float(PIXELS_GUN_LENGTH) / PIXELS_PER_CELL;

        ////////////////////////////////////
        // Параметры игрока
        namespace player {
            const short HEALTH = 16;
            const float SPEED  = 6.0f;
            const float FIRE_COOLDOWN_TIME = 1.8f;

            const sf::IntRect FIRST_IMAGE (0, 0, PIXELS_WIDTH + 2, 
                                                 PIXELS_HEIGHT + PIXELS_GUN_LENGTH);
            const sf::IntRect SECOND_IMAGE(0, 88, PIXELS_WIDTH + 2,
                                                  PIXELS_HEIGHT + PIXELS_GUN_LENGTH);
        }

        ////////////////////////////////////
        // Параметры противников
        namespace enemy {
            const short PER_LEVEL = 9;
            const short PER_LEVEL_IN_ONE_MOMENT = 5;

            // Параметры простых противников
            namespace basic {
                const short HEALTH = 6;
                const float SPEED  = 4.5f;
                const float SPAWN_TIME = 10.0f;
                const float AI_ANALIZER_TIME   = 0.8f;
                const float FIRE_COOLDOWN_TIME = 2.0f;

                const sf::IntRect IMAGE(0, 176, PIXELS_WIDTH + 2,
                                                PIXELS_HEIGHT + PIXELS_GUN_LENGTH);
            }
        }
    }

    ////////////////////////////////////
    // Параметры базы
    namespace base {
        const short COLUMNS = 8;
        const short ROWS    = 2;

        const short HEALTH  = 10;

        const sf::IntRect IMAGE(0, 0, PIXELS_PER_CELL * COLUMNS,
                                      PIXELS_PER_CELL * ROWS);
    }

    /////////////////////////////////////
    // Параметры снарядов
    namespace bullet {
        // Параметры базового снаряда
        namespace basic {
            const short PIXELS_WIDTH  = 6;
            const short PIXELS_HEIGHT = 10;

            const float WIDTH  = float(PIXELS_WIDTH) / PIXELS_PER_CELL;
            const float HEIGHT = float(PIXELS_HEIGHT) / PIXELS_PER_CELL;

            const float SPEED = 20.0f;

            const sf::IntRect IMAGE(0, 264, PIXELS_WIDTH, PIXELS_HEIGHT);
        }
    }
}