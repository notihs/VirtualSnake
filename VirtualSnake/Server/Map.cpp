#include "Map.h"

enum effects {food, ice, grenade, vodka, oil, glue, o_vodka, o_oil, o_glue, numberOfElements}; //numberOfElements always the last one, to get the count!

int numberOfObjects = (MAP_ROWS + MAP_COLUMNS)/4;

void showMap(TCHAR **map) {


	for (int i = 0; i < MAP_ROWS; i++){
		for (int j = 0; j < MAP_COLUMNS; j++){
			tcout << map[i][j];
		}
		tcout << endl;
	}
}

void generateSnakes(TCHAR **map, int nSnakes, Snake * snake) {

	int countSnake = 0;
	int x, y;
	int offset = SNAKE_INITIAL_SIZE - 1;
	int freeCells;
	//int tailHeadXY[2][2]; //   tail head
 						  //X   []   []
						  //Y   []   []

	if (MAX_PLAYERS < 0 || nSnakes < 0) {
		tcout << TEXT("Insira um numero superior a 0 cobras/jogadores");
		return;
	}
	 
	do {

		freeCells = 0;

		y = (rand() % (MAP_ROWS - 2)) + 1;
		x = (rand() % (MAP_COLUMNS - 2)) + 1;

		//tcout << TEXT("entrei no DO!");

			if (x-offset > 0)
			{
				//tcout << TEXT("a verificar se a posicao x:") << x - offset << " y:" << y << " e valida" << endl;

				for (int i = x-offset; i <= x; i++) {
					if (map[y][i] == FLOOR) {
						//tcout << TEXT("Free CELL!");
						freeCells++;
					}
				}
				if (freeCells == SNAKE_INITIAL_SIZE) {
					for (int i = x - offset; i < x; i++) {

						if (i == x - offset) {
							//tailHeadXY[0][y];
							//tailHeadXY[0][i];
							snake->tailPositionX = y;
							snake->tailPositionY = i;
						}
						map[y][i] = (char) (49 + countSnake); //'x'; //Corpo  da cobra //TODO: mudar o corpo da cobra para um id? talvez? 
					}

					map[y][x] = SNAKE_HEAD; //Cabeca da cobra
					//tailHeadXY[1][y];
					//tailHeadXY[1][x];
					snake->headPositionX = y;
					snake->headPositionY = x;
					tcout << TEXT("Cabeca da cobra x:" << x << " y:" << y << endl);
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

	for (int i = 0; i < MAP_ROWS; i++) {
		map[i] = (TCHAR *)malloc(MAP_COLUMNS * sizeof(TCHAR));
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

	generateItems(map);

	//showMap(map);

	return map;
}

void generateItems(TCHAR **map) {

	//enum effects nElements = numberOfElements; //Contains the number of elements 

	int counterOfObjects = 0;
	int x, y;

	do {

		//tcout << counterOfObjects << TEXT(" - ");
		x = (rand() % (MAP_ROWS - 2)) + 1;
		y = (rand() % (MAP_COLUMNS - 2)) + 1;

		//tcout << TEXT("trying with ") << x << TEXT(",") << y << endl;

		if (map[x][y] == FLOOR) {

			switch (rand() % numberOfElements)
			{
			case food:
				/*tcout << x << TEXT(",") << y << TEXT(" food");
				tcout << endl;*/
				map[x][y] = FOOD;
				break;
			case ice:
				/*tcout << x << TEXT(",") << y << TEXT(" ice");
				tcout << endl;*/
				map[x][y] = ICE;
				break;
			case grenade:
				/*tcout << x << TEXT(",") << y << TEXT(" grenade");
				tcout << endl;*/
				map[x][y] = GRENADE;
				break;
			case vodka:
				/*tcout << x << TEXT(",") << y << TEXT(" vodka");
				tcout << endl;*/
				map[x][y] = VODKA;
				break;
			case oil:
				/*tcout << x << TEXT(",") << y << TEXT(" oil");
				tcout << endl;*/
				map[x][y] = OIL;
				break;
			case glue:
				/*tcout << x << TEXT(",") << y << TEXT(" glue");
				tcout << endl;*/
				map[x][y] = GLUE;
				break;
			case o_vodka:
				/*tcout << x << TEXT(",") << y << TEXT(" o_vodka");
				tcout << endl;*/
				map[x][y] = O_VODKA;
				break;
			case o_oil:
				/*tcout << x << TEXT(",") << y << TEXT(" o_oil");
				tcout << endl;*/
				map[x][y] = O_OIL;
				break;
			case o_glue:
				/*tcout << x << TEXT(",") << y << TEXT(" o_glue");
				tcout << endl;*/
				map[x][y] = O_GLUE;
				break;
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

	} while (counterOfObjects < numberOfObjects);
}

//TODO: Replace items erased!