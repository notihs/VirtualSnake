#include "Map.h"

enum effects {food, ice, grenade, vodka, oil, glue, o_vodka, o_oil, o_glue, numberOfElements}; //numberOfElements always the last one, to get the count!

//int numberOfObjects = (MAP_ROWS + MAP_COLUMNS)/4; //TODO: number of objects should not be fixed!

void showMap(TCHAR **map) {


	for (int i = 0; i < MAP_ROWS; i++){
		for (int j = 0; j < MAP_COLUMNS; j++){
			tcout << map[i][j];
		}
		tcout << endl;
	}
}

void generateSnakes(TCHAR **map, int nSnakes, Snake ** snake) {

	int bodyCounter;
	int countSnake = 0;
	int rndColumn, rndRow;
	int offset = SNAKE_INITIAL_SIZE - 1;
	int freeCells;

	if (MAX_PLAYERS < 0 || nSnakes < 0) {
		tcout << TEXT("Insira um numero superior a 0 cobras/jogadores");
		return;
	}
	 
	do {

		freeCells = 0;

		rndRow = (rand() % (MAP_ROWS - 2)) + 1;
		rndColumn = (rand() % (MAP_COLUMNS - 2)) + 1;

		//tcout << TEXT("entrei no DO!");

			if (rndColumn-offset > 0)
			{
				//tcout << TEXT("a verificar se a posicao rndColumn:") << rndColumn - offset << " rndRow:" << rndRow << " e valida" << endl;

				for (int i = rndColumn-offset; i <= rndColumn; i++) {
					if (map[rndRow][i] == FLOOR) {
						//tcout << TEXT("Free CELL!");
						freeCells++;
					}
				}
				if (freeCells == SNAKE_INITIAL_SIZE) {

					bodyCounter = 0;

					for (int i = rndColumn - offset; i < rndColumn; i++) {

						/*if (i == rndColumn - offset) {
							
							snake[countSnake]->tailPositionRow = rndRow;
							snake[countSnake]->tailPositionColumn = i;
						}*/
						

						snake[countSnake]->bodyRow[bodyCounter] = rndRow;
						snake[countSnake]->bodyColumn[bodyCounter] = i;
						bodyCounter++;

						map[rndRow][i] = (char) (SNAKE_BODY + countSnake); //'rndColumn'; //Corpo  da cobra //TODO: mudar o corpo da cobra para um id? talvez? 
						
					}

					snake[countSnake]->bodyRow[bodyCounter] = rndRow;
					snake[countSnake]->bodyColumn[bodyCounter] = rndColumn;

					map[rndRow][rndColumn] = SNAKE_HEAD; //Cabeca da cobra
					
					//snake[countSnake]->headPositionRow = rndRow;
					//snake[countSnake]->headPositionColumn = rndColumn;
					tcout << TEXT("Cabeca da cobra x:" << rndColumn << " y:" << rndRow << endl);
					countSnake++;
				}
				else {
					continue;
				}
				
			}
		
	} //while (countSnake < MAX_PLAYERS);
	while (countSnake < nSnakes);

}

TCHAR ** createMap() {

	TCHAR ** map;

	map = (TCHAR **)malloc(MAP_ROWS * sizeof(TCHAR *));
	if (map == NULL) {
		tcout << TEXT("Erro na alocacao de memoria de TCHAR** (map) !");
		exit(0);
	}

	for (int i = 0; i < MAP_ROWS; i++) {
		map[i] = (TCHAR *)malloc(MAP_COLUMNS * sizeof(TCHAR));
		if (map[i] == NULL) {
			tcout << TEXT("Erro na alocacao de memoria de TCHAR* (map) !");
			exit(0);
		}
	}

	srand(time(NULL));

	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLUMNS; j++) {
			if (i == 0 || j == 0 || i == MAP_ROWS - 1 || j == MAP_COLUMNS - 1) {
				map[i][j] = WALL;//TEXT('#');
			}
			else {
				map[i][j] = FLOOR;//TEXT('.');
			}
		}
	}

	//generateSnakes(map);

	generateItems(map, 0); //Init a 0 porque nao ha objetos em campo ainda! Mais tarde e chamado com o numero de objetos -1

	//showMap(map);

	return map;
}

void generateItems(TCHAR **map, int counterOfObjects) {

	int x, y;

	do {

		x = (rand() % (MAP_ROWS - 2)) + 1;
		y = (rand() % (MAP_COLUMNS - 2)) + 1;

		if (map[x][y] == FLOOR) {

			//switch (rand() % numberOfElements) //TODO: just for testing
			switch(food)
			{
			case food:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" food");
				tcout << endl;*/
				map[x][y] = FOOD;
				break;
			/*case ice:
				
				map[x][y] = ICE;
				break;
			case grenade:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" grenade");
				tcout << endl;
				map[x][y] = GRENADE;
				break;
			case vodka:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" vodka");
				tcout << endl;
				map[x][y] = VODKA;
				break;
			case oil:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" oil");
				tcout << endl;
				map[x][y] = OIL;
				break;
			case glue:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" glue");
				tcout << endl;
				map[x][y] = GLUE;
				break;
			case o_vodka:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" o_vodka");
				tcout << endl;
				map[x][y] = O_VODKA;
				break;
			case o_oil:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" o_oil");
				tcout << endl;
				map[x][y] = O_OIL;
				break;
			case o_glue:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" o_glue");
				tcout << endl;
				map[x][y] = O_GLUE;
				break;*/
			default:
				tcout << x << TEXT(",") << y << TEXT(" default! DEU ASNEIRA! PROCURAR BUG NO MAP.CPP");
				tcout << endl;
				break;
			}
		}
		else {
			counterOfObjects--;
		}

		counterOfObjects++;

	} while (counterOfObjects < NUMBER_MAX_OF_OBJECTS); //TODO: this is the max of objects, for now.. change this to be dynamic!
}

//TODO: Replace items erased!