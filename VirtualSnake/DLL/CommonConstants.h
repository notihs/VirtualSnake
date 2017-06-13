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
#define GRENADE TEXT('R')
#define VODKA TEXT('V')
#define OIL TEXT('O')
#define GLUE TEXT('G')
#define O_VODKA TEXT('Z')
#define O_OIL TEXT('»')
#define O_GLUE TEXT('«')

//#define SNAKE_HEAD TEXT('~')
#define SNAKE_BODY 49

//OBJECTS
#define MAX_NUMBER_OF_OBJECTS (MAP_ROWS * MAP_COLUMNS)/10
#define DEFAULT_SCORE 10

//RARITY OF OBJECTS in %
#define COMMON 80 
#define UNCOMMON 70
#define RARE 60

//SNAKE
#define SNAKE_INITIAL_SIZE 3
#define SNAKE_BASE_SPEED 2
#define SNAKE_DEFAULT_DIRECTION 'r'
#define BASE_SLEEP 2000
#define DEFAULT_EFFECT_TIMER 8

//BUFFERS
#define TAM 100
#define BUFFER_MAPPING (MAP_ROWS * MAP_COLUMNS)

//PIPES
#define PIPE_SERVER TEXT("\\\\.\\pipe\\server") 
#define PIPE_CLIENT TEXT("\\\\.\\pipe\\client") 
#define N_PIPES 4

