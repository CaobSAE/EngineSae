#include "Game.h"

int main(int argc, char** argv) {
	Game game;

	bool succes = game.Initialize();//Variable de validacion de carga de librerias

	if (succes) {
		game.RunLoop();
	}

	game.Shutdown();

	return 0;
}