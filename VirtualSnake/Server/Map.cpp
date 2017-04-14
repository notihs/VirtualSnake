#include "Map.h"

enum effects {vodka, oil, glue, o_vodka, o_oil, o_glue, numberOfElements};


//TODO: modify this!
int numberOfObjects = 5;

void showMap(TCHAR **map) {


	for (int i = 0; i < WIDTH; i++){
		for (int j = 0; j < HEIGHT; j++){
			tcout << map[i][j];
		}
		tcout << endl;
	}
}

TCHAR ** createMap() {

	TCHAR ** map;

	map = (TCHAR **)malloc(WIDTH * sizeof(TCHAR *));

	for (int i = 0; i < WIDTH; i++) {
		map[i] = (TCHAR *)malloc(HEIGHT * sizeof(TCHAR));
	}

	srand(time(NULL));

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (i == 0 || j == 0 || i == WIDTH - 1 || j == HEIGHT - 1) {
				map[i][j] = TEXT('#');
			}
			else {
				map[i][j] = TEXT('.');
			}
		}
	}


	generateItems(map);

	showMap(map);

	return map;
}

void generateItems(TCHAR **map) {

	//enum effects nElements = numberOfElements; //Contains the number of elements 

	int counterOfObjects = 0;
	int x, y;

	do {

		x = (rand() % (WIDTH - 2)) + 1;
		y = (rand() % (HEIGHT - 2)) + 1;

		//tcout << TEXT("trying with ") << x << TEXT(",") << y << endl;

		if (map[x][y] == '.') {

			switch (rand() % numberOfElements)
			{
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
