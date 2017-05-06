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

void generateSnakes(TCHAR **map, int n) {

	int countSnake = 0;
	int x, y;
	int offset = SNAKE_INITIAL_SIZE - 1;
	int freeCells;

	if (n < 0) {
		tcout << TEXT("Insira um numero superior a 0 cobras");
		return;
	}

	do {

		freeCells = 0;

		y = (rand() % (MAP_ROWS - 2)) + 1;
		x = (rand() % (MAP_COLUMNS - 2)) + 1;

		tcout << TEXT("entrei no DO!");

			if (x-offset > 0)
			{
				tcout << TEXT("a verificar se a posicao x:") << x - offset << " y:" << y << " e valida" << endl;

				for (int i = x-offset; i <= x; i++) {
					if (map[y][i] == '.') {
						tcout << TEXT("Free CELL!");
						freeCells++;
					}
				}
				if (freeCells == SNAKE_INITIAL_SIZE) {
					for (int i = x - offset; i < x; i++) {
						map[y][i] = 'x'; //Corpo  da cobra
					}
					map[y][x] = '~'; //Cabeca da cobra
					tcout << TEXT("Cabeca da cobra x:" << x << " y:" << y << endl);
					countSnake++;
				}
				else {
					continue;
				}
				
			}
		
	} while (countSnake < n);

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
				map[i][j] = TEXT('#');
			}
			else {
				map[i][j] = TEXT('.');
			}
		}
	}

	generateSnakes(map, 2);

	generateItems(map);

	showMap(map);

	return map;
}

void generateItems(TCHAR **map) {

	//enum effects nElements = numberOfElements; //Contains the number of elements 

	int counterOfObjects = 0;
	int x, y;

	do {

		tcout << counterOfObjects << TEXT(" - ");
		x = (rand() % (MAP_ROWS - 2)) + 1;
		y = (rand() % (MAP_COLUMNS - 2)) + 1;

		//tcout << TEXT("trying with ") << x << TEXT(",") << y << endl;

		if (map[x][y] == '.') {

			switch (rand() % numberOfElements)
			{
			case food:
				tcout << x << TEXT(",") << y << TEXT(" food");
				tcout << endl;
				map[x][y] = 'F';
				break;
			case ice:
				tcout << x << TEXT(",") << y << TEXT(" ice");
				tcout << endl;
				map[x][y] = 'I';
				break;
			case grenade:
				tcout << x << TEXT(",") << y << TEXT(" grenade");
				tcout << endl;
				map[x][y] = 'g';
				break;
			case vodka:
				tcout << x << TEXT(",") << y << TEXT(" vodka");
				tcout << endl;
				map[x][y] = 'V';
				break;
			case oil:
				tcout << x << TEXT(",") << y << TEXT(" oil");
				tcout << endl;
				map[x][y] = 'O';
				break;
			case glue:
				tcout << x << TEXT(",") << y << TEXT(" glue");
				tcout << endl;
				map[x][y] = 'G';
				break;
			case o_vodka:
				tcout << x << TEXT(",") << y << TEXT(" o_vodka");
				tcout << endl;
				map[x][y] = 'B';
				break;
			case o_oil:
				tcout << x << TEXT(",") << y << TEXT(" o_oil");
				tcout << endl;
				map[x][y] = 'P';
				break;
			case o_glue:
				tcout << x << TEXT(",") << y << TEXT(" o_glue");
				tcout << endl;
				map[x][y] = 'H';
				break;
			default:
				tcout << x << TEXT(",") << y << TEXT(" default");
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