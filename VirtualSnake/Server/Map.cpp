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

void generateSnakes(TCHAR **map, int nSnakes, Snake ** snake) {

	int countSnake = 0;
	int auxColumn, auxRow;
	int offset = SNAKE_INITIAL_SIZE - 1;
	int freeCells;

	if (MAX_PLAYERS < 0 || nSnakes < 0) {
		tcout << TEXT("Insira um numero superior a 0 cobras/jogadores");
		return;
	}
	 
	do {

		freeCells = 0;

		auxRow = (rand() % (MAP_ROWS - 2)) + 1;
		auxColumn = (rand() % (MAP_COLUMNS - 2)) + 1;

		//tcout << TEXT("entrei no DO!");

			if (auxColumn-offset > 0)
			{
				//tcout << TEXT("a verificar se a posicao auxColumn:") << auxColumn - offset << " auxRow:" << auxRow << " e valida" << endl;

				for (int i = auxColumn-offset; i <= auxColumn; i++) {
					if (map[auxRow][i] == FLOOR) {
						//tcout << TEXT("Free CELL!");
						freeCells++;
					}
				}
				if (freeCells == SNAKE_INITIAL_SIZE) {
					for (int i = auxColumn - offset; i < auxColumn; i++) {

						if (i == auxColumn - offset) {
							
							snake[countSnake]->tailPositionRow = auxRow;
							snake[countSnake]->tailPositionColumn = i;
						}
						map[auxRow][i] = (char) (49 + countSnake); //'auxColumn'; //Corpo  da cobra //TODO: mudar o corpo da cobra para um id? talvez? 
					}

					map[auxRow][auxColumn] = SNAKE_HEAD; //Cabeca da cobra
					
					snake[countSnake]->headPositionRow = auxRow;
					snake[countSnake]->headPositionColumn = auxColumn;
					tcout << TEXT("Cabeca da cobra x:" << auxColumn << " y:" << auxRow << endl);
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

		//tcout << TEXT("trying with ") << auxColumn << TEXT(",") << auxRow << endl;

		if (map[x][y] == FLOOR) {

			switch (rand() % numberOfElements)
			{
			case food:
				/*tcout << auxColumn << TEXT(",") << auxRow << TEXT(" food");
				tcout << endl;*/
				map[x][y] = FOOD;
				break;
			case ice:
				/*tcout << auxColumn << TEXT(",") << auxRow << TEXT(" ice");
				tcout << endl;*/
				map[x][y] = ICE;
				break;
			case grenade:
				/*tcout << auxColumn << TEXT(",") << auxRow << TEXT(" grenade");
				tcout << endl;*/
				map[x][y] = GRENADE;
				break;
			case vodka:
				/*tcout << auxColumn << TEXT(",") << auxRow << TEXT(" vodka");
				tcout << endl;*/
				map[x][y] = VODKA;
				break;
			case oil:
				/*tcout << auxColumn << TEXT(",") << auxRow << TEXT(" oil");
				tcout << endl;*/
				map[x][y] = OIL;
				break;
			case glue:
				/*tcout << auxColumn << TEXT(",") << auxRow << TEXT(" glue");
				tcout << endl;*/
				map[x][y] = GLUE;
				break;
			case o_vodka:
				/*tcout << auxColumn << TEXT(",") << auxRow << TEXT(" o_vodka");
				tcout << endl;*/
				map[x][y] = O_VODKA;
				break;
			case o_oil:
				/*tcout << auxColumn << TEXT(",") << auxRow << TEXT(" o_oil");
				tcout << endl;*/
				map[x][y] = O_OIL;
				break;
			case o_glue:
				/*tcout << auxColumn << TEXT(",") << auxRow << TEXT(" o_glue");
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