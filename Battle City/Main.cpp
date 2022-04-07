#include "Game.h"


///////////////////////
// Точка входа
int main() {
    std::unique_ptr<Game> game(new Game());

    game->setupSystem();
    game->initialize();

    while (game->loop());

    return 0;
}