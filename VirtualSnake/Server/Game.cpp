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
		game->snake[i]->bodyRow = (int *)malloc(SNAKE_INITIAL_SIZE * sizeof(int));
		game->snake[i]->bodyColumn = (int *)malloc(SNAKE_INITIAL_SIZE * sizeof(int));

		for (int j = 0; j < SNAKE_INITIAL_SIZE; j++) {
			game->snake[i]->bodyRow[j] = -1;
			game->snake[i]->bodyColumn[j] = -1;
		}
		/*
		game->snake[i]->tailPositionRow = -1;
		game->snake[i]->tailPositionColumn = -1;
		game->snake[i]->headPositionRow = -1;
		game->snake[i]->headPositionColumn = -1;*/
	}

}

void showSnakeInfo() {

	for (int i = 0; i < game->activePlayers; i++) {

		tcout << TEXT("ID ->") << game->snake[i]->id << endl;
		tcout << TEXT("ALIVE ->") << game->snake[i]->alive << endl;
		tcout << TEXT("SIZE ->") << game->snake[i]->size << endl;
		tcout << TEXT("SPEED ->") << game->snake[i]->speed << endl;
		/*tcout << TEXT("HEAD POSITION X ->") << game->snake[i]->headPositionRow << endl;
		tcout << TEXT("HEAD POSITION Y ->") << game->snake[i]->headPositionColumn << endl;
		tcout << TEXT("TAIL POSITION X ->") << game->snake[i]->tailPositionRow << endl;
		tcout << TEXT("TAIL POSITION Y ->") << game->snake[i]->tailPositionColumn << endl;*/
		tcout << TEXT("BODY POSITIONS: ") << endl;

		for (int j = 0; j < game->snake[j]->size; j++) {
			tcout << TEXT("ROW ") << j << TEXT(" ->") << game->snake[i]->bodyRow[j] << endl;
			tcout << TEXT("COLUMN ") << j << TEXT(" ->") << game->snake[i]->bodyColumn[j] << endl;
		}

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
	
	int snakeSize = game->snake[player]->size;
	int headPosColumn = game->snake[player]->bodyColumn[snakeSize-1];
	int headPosRow = game->snake[player]->bodyRow[snakeSize-1];

	if (keyPressed == 'u') {
		if (game->map[headPosRow -1][headPosColumn] == FLOOR) {
			//tcout << TEXT("TUDO OK! Jogador ") << player;

			standardMovement(player, -1, 0);

		}
	}
	else if (keyPressed == 'd') {
		if (game->map[headPosRow + 1][headPosColumn] == FLOOR) {
			//tcout << TEXT("TUDO OK! Jogador ") << player;

			standardMovement(player, 1, 0);
		}
	}
	else if (keyPressed == 'l') {
		if (game->map[headPosRow][headPosColumn -1] == FLOOR) {
			//tcout << TEXT("TUDO OK! Jogador ") << player;

			standardMovement(player, 0, -1);
	
		}
	}
	else if (keyPressed == 'r') {
		if (game->map[headPosRow][headPosColumn + 1] == FLOOR) {
			//tcout << TEXT("TUDO OK! Jogador ") << player;
			standardMovement(player, 0, 1);
	
		}
	}

	showSnakeInfo();
	showMap(game->map);
	writeMapInMemory(game->map);
}

void standardMovement(int player, int rowMovement, int columnMovement) {

	int snakeSize = game->snake[player]->size;
	int tailPosColumn = game->snake[player]->bodyColumn[0];
	int tailPosRow = game->snake[player]->bodyRow[0];
	int headPosColumn = game->snake[player]->bodyColumn[snakeSize - 1];
	int headPosRow = game->snake[player]->bodyRow[snakeSize - 1];

	//DESENHO DO MAPA
	game->map[headPosRow + rowMovement][headPosColumn + columnMovement] = SNAKE_HEAD;
	game->map[headPosRow][headPosColumn] = (char)(SNAKE_BODY + game->snake[player]->id);
	game->map[tailPosRow][tailPosColumn] = FLOOR;

	//LOGICA DA COBRA 
	for (int i = 0; i < snakeSize - 1; i++) {
		game->snake[player]->bodyRow[i] = game->snake[player]->bodyRow[i + 1];
		game->snake[player]->bodyColumn[i] = game->snake[player]->bodyColumn[i + 1];
	}

	game->snake[player]->bodyRow[snakeSize - 1] = headPosRow + rowMovement;
	game->snake[player]->bodyColumn[snakeSize - 1] = headPosColumn + columnMovement;
	
}