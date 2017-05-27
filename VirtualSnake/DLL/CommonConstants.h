#pragma once

#define MAX_PLAYERS 4

//MAP
#define MAP_ROWS 10
#define MAP_COLUMNS 20

//SNAKE
#define SNAKE_INITIAL_SIZE 3
#define SNAKE_BASE_SPEED 1

//BUFFERS
#define TAM 100
#define BUFFER_MAPPING (MAP_ROWS * MAP_COLUMNS)

//PIPES
#define PIPE_SERVER TEXT("\\\\.\\pipe\\server") 
#define N_PIPES 4