#include "Map.h"

enum effects {food, ice, grenade, vodka, oil, glue, o_vodka, o_oil, o_glue, numberOfElements}; //numberOfElements always the last one, to get the count!

//int numberOfObjects = (MAP_ROWS + MAP_COLUMNS)/4; 

void showMap(TCHAR **map) {


	for (int i = 0; i < MAP_ROWS; i++){
		for (int j = 0; j < MAP_COLUMNS; j++){
			//tcout << map[i][j];
			_tprintf(TEXT("%c"), map[i][j]);
		}
		//tcout << endl;
		_tprintf(TEXT("\n"));
	}
}

void generateSnakes(TCHAR **map, int nSnakes, Snake ** snake) {

	int bodyCounter;
	int countSnake = 0;
	int rndColumn, rndRow;
	int offset = SNAKE_INITIAL_SIZE - 1;
	int freeCells;

	if (MAX_PLAYERS < 0 || nSnakes < 0) {
		//tcout << TEXT("Insira um numero superior a 0 cobras/jogadores");
		_tprintf(TEXT("Insira um numero superior a 0 cobras/jogadores"));
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

						map[rndRow][i] = (char) (SNAKE_BODY + countSnake); //'rndColumn'; //Corpo  da cobra 
						
					}

					snake[countSnake]->bodyRow[bodyCounter] = rndRow;
					snake[countSnake]->bodyColumn[bodyCounter] = rndColumn;

					//map[rndRow][rndColumn] = SNAKE_HEAD; //Cabeca da cobra com simbolo diferente do corpo!
					map[rndRow][rndColumn] = (char)(SNAKE_BODY + countSnake); //Cabeca da cobra
					
					//snake[countSnake]->headPositionRow = rndRow;
					//snake[countSnake]->headPositionColumn = rndColumn;
					//tcout << TEXT("Cabeca da cobra x:" << rndColumn << " y:" << rndRow << endl);
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
		//tcout << TEXT("Erro na alocacao de memoria de TCHAR** (map) !");
		_tprintf(TEXT("Erro na alocacao de memoria de TCHAR** (map) !"));
		exit(0);
	}

	for (int i = 0; i < MAP_ROWS; i++) {
		map[i] = (TCHAR *)malloc(MAP_COLUMNS * sizeof(TCHAR));
		if (map[i] == NULL) {
			//tcout << TEXT("Erro na alocacao de memoria de TCHAR* (map) !");
			_tprintf(TEXT("Erro na alocacao de memoria de TCHAR* (map) !"));
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

	int x, y, rarity;

	do {

		x = (rand() % (MAP_ROWS - 2)) + 1;
		y = (rand() % (MAP_COLUMNS - 2)) + 1;

		if (map[x][y] == FLOOR) {

			rarity = rand() % 101;
			int res = rand() % numberOfElements;

//			tcout << TEXT("raridade: ") << rarity << TEXT(" em item ") << res << endl;

			

			switch (res) 
			{
			case food:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" food");
				tcout << endl;*/
				if (rarity <= COMMON) {
					map[x][y] = FOOD;
				}
				else {
					counterOfObjects--;
				}
				
				break;
			case ice:
				if (rarity <= UNCOMMON) {
					map[x][y] = ICE;
				}
				else {
					counterOfObjects--;
				}
				break;
			case grenade:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" grenade");
				tcout << endl;*/
				if (rarity <= RARE) {
					map[x][y] = GRENADE;
				}
				else {
					counterOfObjects--;
				}
				break;
			case vodka:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" vodka");
				tcout << endl;*/
				if (rarity <= RARE) {
					map[x][y] = VODKA;
				}
				else {
					counterOfObjects--;
				}
				break;
			case oil:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" oil");
				tcout << endl;*/
				if (rarity <= UNCOMMON) {
					map[x][y] = OIL;
				}
				else {
					counterOfObjects--;
				}
				break;
				break;
			case glue:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" glue");
				tcout << endl;*/
				if (rarity <= UNCOMMON) {
					map[x][y] = GLUE;
				}
				else {
					counterOfObjects--;
				}
				break;
				break;
			case o_vodka:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" o_vodka");
				tcout << endl;*/
				if (rarity <= RARE) {
					map[x][y] = O_VODKA;
				}
				else {
					counterOfObjects--;
				}
				break;
			case o_oil:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" o_oil");
				tcout << endl;*/
				if (rarity <= RARE) {
					map[x][y] = O_OIL;
				}
				else {
					counterOfObjects--;
				}
				break;
			case o_glue:
				/*tcout << rndColumn << TEXT(",") << rndRow << TEXT(" o_glue");
				tcout << endl;*/
				if (rarity <= RARE) {
					map[x][y] = O_GLUE;
				}
				else {
					counterOfObjects--;
				}
				break;
			default:
				//tcout << x << TEXT(",") << y << TEXT(" default! DEU ASNEIRA! PROCURAR BUG NO MAP.CPP");
				_tprintf(TEXT("%d, %d default! DEU ASNEIRA! PROCURAR BUG NO MAP.CPP\n"), x, y);
				//tcout << endl;
				break;
			}
		}
		else {
			counterOfObjects--;
		}

		counterOfObjects++;

	} while (counterOfObjects < MAX_NUMBER_OF_OBJECTS); 
}

