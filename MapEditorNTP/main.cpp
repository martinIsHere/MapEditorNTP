#define SDL_MAIN_HANDLED
#include "iostream"
#include "Game.h"



int main() {
	Game* game = new Game();
	game->init("Editor",1400, 800);

	Uint32 startTick;
	int endTick;

	const int fps = 60;
	const int delay = 1000 / fps;

	while (game->running) {
		startTick = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->draw();


		endTick = SDL_GetTicks() - startTick;
		if (endTick < delay) {
			SDL_Delay(delay - endTick);
		}
	}
	game->clean();
	SDL_Quit();

	return 0;
}