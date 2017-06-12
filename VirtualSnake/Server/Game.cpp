#include "Game.h"

Game * game;

void initGame() {
	game = (Game *)malloc(sizeof(Game));

	if (game == NULL) {
		tcout << TEXT("Erro na alocacao de memoria de Game* !");
		exit(0);
	}

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
	
	Game * aux = game;
	return aux;

}

void initSnakes() {

	game->snake = (Snake **)malloc(game->activePlayers * sizeof(Snake *));
	if (game->snake == NULL) {
		tcout << TEXT("Erro na alocacao de memoria de Snake** !");
		exit(0);
	}

	for (int i = 0; i < MAX_PLAYERS; i++) {

		game->snake[i] = (Snake *)malloc(sizeof(Snake));
		if (game->snake[i] == NULL) {
			tcout << TEXT("Erro na alocacao de memoria de Snake* !");
			exit(0);
		}

		game->snake[i]->alive = true;
		game->snake[i]->id = i;
		game->snake[i]->size = SNAKE_INITIAL_SIZE;
		game->snake[i]->speed = SNAKE_BASE_SPEED;
		game->snake[i]->direction = SNAKE_DEFAULT_DIRECTION;
		game->snake[i]->isActive = false;
		game->snake[i]->isDrunk = false;
		game->snake[i]->score = 0;

		//Valores invalidos!!!! Passam a validos apos startGame
		game->snake[i]->bodyRow = (int *)malloc(SNAKE_INITIAL_SIZE * sizeof(int));
		if (game->snake[i]->bodyRow == NULL) {
			tcout << TEXT("Erro na alocacao de memoria de int* (bodyRow) !");
			exit(0);
		}
		game->snake[i]->bodyColumn = (int *)malloc(SNAKE_INITIAL_SIZE * sizeof(int));
		if (game->snake[i]->bodyColumn == NULL) {
			tcout << TEXT("Erro na alocacao de memoria de int* (bodyColumn) !");
			exit(0);
		}

		for (int j = 0; j < SNAKE_INITIAL_SIZE; j++) {
			game->snake[i]->bodyRow[j] = -1;
			game->snake[i]->bodyColumn[j] = -1;
		}

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

	if (game->activePlayers > 0) {
		//tcout << TEXT("Jogadores ativos! -> ") << game->activePlayers;

		generateSnakes(game->map, game->activePlayers, game->snake); //Tail & Head position

		//initSnakes(game->snake, game->activePlayers); //All the rest!


		//showMap(game->map);
	} 
}

bool validMovement(int player, TCHAR keyPressed) {

	int snakeSize = game->snake[player]->size;
	int headPosColumn = game->snake[player]->bodyColumn[snakeSize - 1];
	int headPosRow = game->snake[player]->bodyRow[snakeSize - 1];
	int beforeHeadPosColumn = game->snake[player]->bodyColumn[snakeSize - 2];
	int beforeHeadPosRow = game->snake[player]->bodyRow[snakeSize - 2];

	if (keyPressed == 'u') {
		if (beforeHeadPosRow == headPosRow - 1 && beforeHeadPosColumn == headPosColumn) {
			return false;
		}
	}
	else if (keyPressed == 'd') {
		if (beforeHeadPosRow == headPosRow + 1 && beforeHeadPosColumn == headPosColumn) {
			return false;
		}
	}
	else if (keyPressed == 'l') {
		if (beforeHeadPosRow == headPosRow && beforeHeadPosColumn == headPosColumn - 1) {
			return false;
		}
	}
	else if (keyPressed == 'r') {
		if (beforeHeadPosRow == headPosRow && beforeHeadPosColumn == headPosColumn + 1) {
			return false;
		}
	}

	return true;
}

void tryToMoveSnake(int player, TCHAR keyPressed) {

	int snakeSize = game->snake[player]->size;
	int headPosColumn = game->snake[player]->bodyColumn[snakeSize - 1];
	int headPosRow = game->snake[player]->bodyRow[snakeSize - 1];
	TCHAR nextPosition;

	if (keyPressed == 'u') {
		nextTile(player, -1, 0);
	}
	else if (keyPressed == 'd') { // (1,0)
		nextTile(player, 1, 0);
	}
	else if (keyPressed == 'l') { //(0, -1)
		nextTile(player, 0, -1);
	}
	else if (keyPressed == 'r') {
		nextTile(player, 0, 1);
	}

	//showSnakeInfo();
	showMap(game->map);
	writeMapInMemory(game->map);

}

void nextTile(int player, int row, int column) {

	int snakeSize = game->snake[player]->size;
	int headPosColumn = game->snake[player]->bodyColumn[snakeSize - 1];
	int headPosRow = game->snake[player]->bodyRow[snakeSize - 1];
	TCHAR nextPosition;

	nextPosition = game->map[headPosRow + row][headPosColumn + column];

	switch (nextPosition) {
	case FLOOR:
		standardMovement(player, row, column);
		break;

	case FOOD:
		eatingMovement(player, row, column);
		break;

	case OIL:
		eatingOil(player, row, column);
		break;

	case GLUE:
		eatingGlue(player, row, column);
		break;

	case GRENADE:
		eatingGrenade(player, row, column);
		break;

	case WALL:
		hitWall(player);
		break;

	case O_GLUE:
		opponentGlued(player, row, column);
		break;

	case O_OIL:
		opponentOiled(player, row, column);
		break;

	case VODKA:
		drinkVodka(player, row, column);
		break;

	case O_VODKA:
		opponentDrunk(player, row, column);
		break;

	case ICE:
		getFrozen(player, row, column);
		break;

	default:
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (nextPosition == (char)(SNAKE_BODY + i)) {
				killSnake(player);
				if (i != game->snake[player]->id) {
					game->snake[i]->score += 5 * DEFAULT_SCORE;
				}
				break;
			}
		}
		break;
	}

}

void standardMovement(int player, int rowMovement, int columnMovement) {

	int snakeSize = game->snake[player]->size;
	int tailPosColumn = game->snake[player]->bodyColumn[0];
	int tailPosRow = game->snake[player]->bodyRow[0];
	int headPosColumn = game->snake[player]->bodyColumn[snakeSize - 1];
	int headPosRow = game->snake[player]->bodyRow[snakeSize - 1];

	//DESENHO DO MAPA
	//game->map[headPosRow + rowMovement][headPosColumn + columnMovement] = SNAKE_HEAD;
	game->map[headPosRow + rowMovement][headPosColumn + columnMovement] = (char)(SNAKE_BODY + game->snake[player]->id);
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

void eatingMovement(int player, int rowMovement, int columnMovement) { 

	int snakeSize = game->snake[player]->size;
	int tailPosColumn = game->snake[player]->bodyColumn[0];
	int tailPosRow = game->snake[player]->bodyRow[0];
	int headPosColumn = game->snake[player]->bodyColumn[snakeSize - 1];
	int headPosRow = game->snake[player]->bodyRow[snakeSize - 1];

	//DESENHO DO MAPA

	//game->map[headPosRow + rowMovement][headPosColumn + columnMovement] = SNAKE_HEAD;
	game->map[headPosRow + rowMovement][headPosColumn + columnMovement] = (char)(SNAKE_BODY + game->snake[player]->id);
	game->map[headPosRow][headPosColumn] = (char)(SNAKE_BODY + game->snake[player]->id);


	//LOGICA DA COBRA 

	int * auxRow = (int *)malloc(sizeof(int) * snakeSize);
	int * auxColumn = (int *)malloc(sizeof(int) * snakeSize);

	for (int i = 0; i < snakeSize; i++) {
		auxRow[i] = game->snake[player]->bodyRow[i];
		auxColumn[i] = game->snake[player]->bodyColumn[i];
	}

	game->snake[player]->size = snakeSize + 1;
	snakeSize = game->snake[player]->size;

	game->snake[player]->bodyColumn = (int *)realloc(game->snake[player]->bodyColumn, snakeSize);
	game->snake[player]->bodyRow = (int *)realloc(game->snake[player]->bodyRow, snakeSize);

	for (int i = 0; i < snakeSize - 1; i++) {
		game->snake[player]->bodyRow[i] = auxRow[i];
		game->snake[player]->bodyColumn[i] = auxColumn[i];
	}

	game->snake[player]->bodyRow[snakeSize - 1] = headPosRow + rowMovement;
	game->snake[player]->bodyColumn[snakeSize - 1] = headPosColumn + columnMovement;


	generateItems(game->map, MAX_NUMBER_OF_OBJECTS - 1);

	//SCORE

	if (game->snake[player]->isDrunk) {
		game->snake[player]->score += 3 * DEFAULT_SCORE;
	}
	else if (game->snake[player]->speed > SNAKE_BASE_SPEED) {
		game->snake[player]->score += 2 * DEFAULT_SCORE;
	}
	else if (game->snake[player]->speed < SNAKE_BASE_SPEED) {
		game->snake[player]->score += 2 / DEFAULT_SCORE;
	}
	else {
		game->snake[player]->score += DEFAULT_SCORE;
	}

	free(auxColumn);
	free(auxRow);
}

void getFrozen(int player, int rowMovement, int columnMovement) {

	int snakeSize = game->snake[player]->size;
	int tailPosColumn = game->snake[player]->bodyColumn[1];
	int tailPosRow = game->snake[player]->bodyRow[1];


	standardMovement(player, rowMovement, columnMovement);

	if (snakeSize > SNAKE_INITIAL_SIZE) {

		//DESENHO DO MAPA
		game->map[tailPosRow][tailPosColumn] = FLOOR;

		//LOGICA 
		game->snake[player]->size = snakeSize - 1;
		snakeSize = game->snake[player]->size;

		int * auxRow = (int *)malloc(sizeof(int) * snakeSize);
		int * auxColumn = (int *)malloc(sizeof(int) * snakeSize);

		for (int i = 0; i < snakeSize; i++) {
			auxRow[i] = game->snake[player]->bodyRow[i + 1];
			auxColumn[i] = game->snake[player]->bodyColumn[i + 1];
		}

		game->snake[player]->bodyColumn = (int *)realloc(game->snake[player]->bodyColumn, snakeSize);
		game->snake[player]->bodyRow = (int *)realloc(game->snake[player]->bodyRow, snakeSize);

		for (int i = 0; i < snakeSize; i++) {
			game->snake[player]->bodyRow[i] = auxRow[i];
			game->snake[player]->bodyColumn[i] = auxColumn[i];
		}

	}

	//SCORE
	if (game->snake[player]->score >= DEFAULT_SCORE) {
		game->snake[player]->score -= DEFAULT_SCORE;
	}
	else if (game->snake[player]->score < DEFAULT_SCORE) {
		game->snake[player]->score = 0;
	}


	generateItems(game->map, MAX_NUMBER_OF_OBJECTS - 1);
}

void eatingOil(int player, int rowMovement, int columnMovement) {

	standardMovement(player, rowMovement, columnMovement);
	game->snake[player]->speed = SNAKE_BASE_SPEED * 2;
	generateItems(game->map, MAX_NUMBER_OF_OBJECTS - 1);

}

void eatingGlue(int player, int rowMovement, int columnMovement) {

	standardMovement(player, rowMovement, columnMovement);
	game->snake[player]->speed = SNAKE_BASE_SPEED / 2;
	generateItems(game->map, MAX_NUMBER_OF_OBJECTS - 1);

}

void eatingGrenade(int player, int rowMovement, int columnMovement) {

	int snakeSize = game->snake[player]->size;
	int headPosColumn = game->snake[player]->bodyColumn[snakeSize - 1];
	int headPosRow = game->snake[player]->bodyRow[snakeSize - 1];

	game->map[headPosRow + rowMovement][headPosColumn + columnMovement] = FLOOR;

	killSnake(player);
	generateItems(game->map, MAX_NUMBER_OF_OBJECTS - 1);

}

void hitWall(int player) {

	killSnake(player);

}

void killSnake(int player) {

	int snakeSize = game->snake[player]->size;
	int posBodyRow;
	int posBodyColumn;

	//DESENHO 

	for (int i = 0; i < snakeSize; i++) {
		posBodyRow = game->snake[player]->bodyRow[i];
		posBodyColumn = game->snake[player]->bodyColumn[i];

		game->map[posBodyRow][posBodyColumn] = FLOOR;
	}

	//LOGICA 

	for (int i = 0; i < snakeSize; i++) {

		game->snake[player]->bodyColumn[i] = -1;
		game->snake[player]->bodyRow[i] = -1;
	}

	game->snake[player]->alive = false;
}

void opponentGlued(int player, int rowMovement, int columnMovement) {

	standardMovement(player, rowMovement, columnMovement);

	for (int i = 0; i < game->activePlayers; i++) {

		if (i != player) {
			game->snake[i]->speed /= 2;
		}
	}

	generateItems(game->map, MAX_NUMBER_OF_OBJECTS - 1);

}

void opponentOiled(int player, int rowMovement, int columnMovement) {

	standardMovement(player, rowMovement, columnMovement);

	for (int i = 0; i < game->activePlayers; i++) {

		if (i != player) {
			game->snake[i]->speed *= 2;
		}
	}

	generateItems(game->map, MAX_NUMBER_OF_OBJECTS - 1);
}

void drinkVodka(int player, int rowMovement, int columnMovement) {

	standardMovement(player, rowMovement, columnMovement);
	game->snake[player]->isDrunk = true;
	generateItems(game->map, MAX_NUMBER_OF_OBJECTS - 1);
}

void opponentDrunk(int player, int rowMovement, int columnMovement) {

	standardMovement(player, rowMovement, columnMovement);

	for (int i = 0; i < game->activePlayers; i++) {

		if (i != player) {
			game->snake[i]->isDrunk = true;
		}
	}

	generateItems(game->map, MAX_NUMBER_OF_OBJECTS - 1);

}

TCHAR swapKeys(TCHAR key) {
	if (key == 'u') { return 'd'; }
	else if (key == 'd') { return 'u'; }
	else if (key == 'l') { return 'r'; }
	else if (key == 'r') { return 'l'; }

	return ' '; //Just in case
}