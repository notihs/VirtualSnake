#include "Game.h"

void initGame(Game *game) {

	/*for (int i = 0; i < MAX_PLAYERS; i++) {
		game->snake[i] = initOneSnake(i); //Snake.h
	}*/
	game->hasStarted = false;
	game->activePlayers = 0;
	game->map = createMap();

	return;
}
/*
Snake initOneSnake(int id) {

	Snake snake;

	snake.alive = true;
	snake.headPositionX = -1;
	snake.headPositionY = -1;
	snake.tailPositionX = -1;
	snake.tailPositionY = -1;
	snake.id = id;
	snake.size = SNAKE_INITIAL_SIZE;
	snake.speed = SNAKE_BASE_SPEED;

	return snake;
}
*/
void initSnakes(Snake *snake, int activePlayers) {

	for (int i = 0; i < activePlayers; i++) {
	
		snake->alive = true;
		snake->id = i;
		snake->size = SNAKE_INITIAL_SIZE;
		snake->speed = SNAKE_BASE_SPEED;
	}

}

void showSnakeInfo(Game *game) {

	for (int i = 0; i < game->activePlayers; i++) {

		tcout << TEXT("ID ->") << game->snake->id << endl;
		tcout << TEXT("ALIVE ->") << game->snake->alive << endl;
		tcout << TEXT("SIZE ->") << game->snake->size << endl;
		tcout << TEXT("SPEED ->") << game->snake->speed << endl;
		tcout << TEXT("HEAD POSITION X ->") << game->snake->headPositionX << endl;
		tcout << TEXT("HEAD POSITION Y ->") << game->snake->headPositionY << endl;
		tcout << TEXT("TAIL POSITION X ->") << game->snake->tailPositionX << endl;
		tcout << TEXT("TAIL POSITION Y ->") << game->snake->tailPositionY << endl;

	}

}

void startGame(Game *game) {

	

	if(game->activePlayers > 0){
		tcout << TEXT("Jogadores ativos! -> ") << game->activePlayers;

		game->snake = (Snake *)malloc(game->activePlayers * sizeof(Snake));

		generateSnakes(game->map, game->activePlayers, game->snake); //Tail & Head position

		initSnakes(game->snake, game->activePlayers); //All the rest!

		
		//showMap(game->map);
	}
}

void tryToMoveSnake(Game * game, int player, TCHAR keyPressed) {
	
	int headPosX = game->snake[player].headPositionX;
	int headPosY = game->snake[player].headPositionY;

	if (keyPressed == 'u') {
		
		if (headPosY - 1 == FLOOR) {
			tcout << TEXT("TUDO OK! Jogador ") << player;
		}
	}
	else if (keyPressed == 'd') {
		if (headPosY + 1 == FLOOR) {
			tcout << TEXT("TUDO OK! Jogador ") << player;
		}
	}
	else if (keyPressed == 'l') {
		if (headPosX - 1 == FLOOR) {
			tcout << TEXT("TUDO OK! Jogador ") << player;
		}
	}
	else if (keyPressed == 'r') {
		if (headPosX + 1 == FLOOR) {
			tcout << TEXT("TUDO OK! Jogador ") << player;
		}
	}

}