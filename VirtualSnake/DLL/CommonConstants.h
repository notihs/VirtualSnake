#pragma once

#define MAX_PLAYERS 4

//MAP
#define MAP_ROWS 10
#define MAP_COLUMNS 20

//MAP TILES
#define WALL TEXT('#')
#define FLOOR TEXT('.')
#define FOOD TEXT('F')
#define ICE TEXT('I')
#define GRENADE TEXT('g')
#define VODKA TEXT('V')
#define OIL TEXT('O')
#define GLUE TEXT('G')
#define O_VODKA TEXT('B')
#define O_OIL TEXT('P')
#define O_GLUE TEXT('H')
#define SNAKE_HEAD TEXT('~')
#define SNAKE_BODY 49

#define NUMBER_MAX_OF_OBJECTS (MAP_ROWS + MAP_COLUMNS)/3

//SNAKE
#define SNAKE_INITIAL_SIZE 3
#define SNAKE_BASE_SPEED 1
#define SNAKE_DEFAULT_DIRECTION 'r'

//BUFFERS
#define TAM 100
#define BUFFER_MAPPING (MAP_ROWS * MAP_COLUMNS)

//PIPES
#define PIPE_SERVER TEXT("\\\\.\\pipe\\server") 
#define N_PIPES 4

