#pragma once

#define MAP_ROWS 10
#define MAP_COLUMNS 20
#define SNAKE_INITIAL_SIZE 3
#define SNAKE_BASE_SPEED 1
#define MAX_PLAYERS 4

#define PIPE_SERVER TEXT("\\\\.\\pipe\\server") // do server, para o cliente (leitura/escrita)
//#define PIPE_CS TEXT("\\\\.\\pipe\\testeCS") // do cliente, para o servidor (leitura/escrita)

//Tamanho do buffer de leitura
//#define TAMx 256
//Numero de clientes ligados em simultaneo ao server
#define N_PIPES 4


// HANDLES PARA OS PIPES 
//HANDLE hPipeRead[N_PIPES];
//HANDLE hPipeWrite[N_PIPES]; 

