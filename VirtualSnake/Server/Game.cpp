#include "Game.h"

Game * game;

void initGame() {
	game = (Game *)malloc(sizeof(Game));
	/*for (int i = 0; i < MAX_PLAYERS; i++) {
		game->snake[i] = initOneSnake(i); //Snake.h
	}*/
	game->hasStarted = false;
	game->activePlayers = 0;
	game->map = createMap();
	initSnakes();
	//currentGame = game;
	return;
}

Game * getCurrentGame() {
	//TODO: add mutex to protect currentGame values
	Game * aux = game; //TODO: de certeza que nao vai dar asneira?? hmmmmmm...
	return aux;

}

void initSnakes() {

	game->snake = (Snake **)malloc(game->activePlayers * sizeof(Snake *));

	for (int i = 0; i < MAX_PLAYERS; i++) {
		
		game->snake[i] = (Snake *)malloc(sizeof(Snake));
		
		game->snake[i]->alive = true;
		game->snake[i]->id = i;
		game->snake[i]->size = SNAKE_INITIAL_SIZE;
		game->snake[i]->speed = SNAKE_BASE_SPEED;

		//Valores invalidos!!!! Passam a validos apos startGame
		game->snake[i]->tailPositionRow = -1;
		game->snake[i]->tailPositionColumn = -1;
		game->snake[i]->headPositionRow = -1;
		game->snake[i]->headPositionColumn = -1;
	}

}

void showSnakeInfo() {

	for (int i = 0; i < game->activePlayers; i++) {

		tcout << TEXT("ID ->") << game->snake[i]->id << endl;
		tcout << TEXT("ALIVE ->") << game->snake[i]->alive << endl;
		tcout << TEXT("SIZE ->") << game->snake[i]->size << endl;
		tcout << TEXT("SPEED ->") << game->snake[i]->speed << endl;
		tcout << TEXT("HEAD POSITION X ->") << game->snake[i]->headPositionRow << endl;
		tcout << TEXT("HEAD POSITION Y ->") << game->snake[i]->headPositionColumn << endl;
		tcout << TEXT("TAIL POSITION X ->") << game->snake[i]->tailPositionRow << endl;
		tcout << TEXT("TAIL POSITION Y ->") << game->snake[i]->tailPositionColumn << endl;

	}

}

void startGame() {

	if(game->activePlayers > 0){
		tcout << TEXT("Jogadores ativos! -> ") << game->activePlayers;

		generateSnakes(game->map, game->activePlayers, game->snake); //Tail & Head position

		//initSnakes(game->snake, game->activePlayers); //All the rest!

		
		//showMap(game->map);
	} //TODO: as cobras foram inicializadas! atencao as que tenham -1 na tail etc, essas nao foram usadas
}

void tryToMoveSnake(int player, TCHAR keyPressed) {
	
	int headPosColumn = game->snake[player]->headPositionColumn;
	int headPosRow = game->snake[player]->headPositionRow;

	//showMap(game->map);

	if (keyPressed == 'u') {
		
		if (game->map[headPosRow -1][headPosColumn] == FLOOR) {
			tcout << TEXT("TUDO OK! Jogador ") << player;

			game->map[headPosRow - 1][headPosColumn] = SNAKE_HEAD;
			game->map[headPosRow][headPosColumn] = (char)(SNAKE_BODY + game->snake[player]->id);
			game->map[game->snake[player]->tailPositionRow][game->snake[player]->tailPositionColumn] = FLOOR;

			game->snake[player]->headPositionRow = headPosRow - 1;
			game->snake[player]->headPositionColumn = headPosColumn;
			//game->snake[player]->tailPositionRow = ; //TODO: fix tail position
			//game->snake[player]->tailPositionColumn = ;
		}
	}
	else if (keyPressed == 'd') {
		if (game->map[headPosRow + 1][headPosColumn] == FLOOR) {
			tcout << TEXT("TUDO OK! Jogador ") << player;
		}
	}
	else if (keyPressed == 'l') {
		if (game->map[headPosRow][headPosColumn -1] == FLOOR) {
			tcout << TEXT("TUDO OK! Jogador ") << player;
		}
	}
	else if (keyPressed == 'r') {
		if (game->map[headPosRow][headPosColumn + 1] == FLOOR) {
			tcout << TEXT("TUDO OK! Jogador ") << player;
		}
	}

}