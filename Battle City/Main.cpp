#include "Game.h"


///////////////////////
// Точка входа
int main()
{
	Game* game = new Game();

	//do {
		game->setupSystem();
		game->initialize();

		while (game->loop());

	//} while (!game->shutdown());

	delete game;
	game = nullptr;

	return 0;
}