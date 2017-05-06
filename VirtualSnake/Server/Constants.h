                                     #pragma once

#define MAP_ROWS 10
#define MAP_COLUMNS 20
#define SNAKE_INITIAL_SIZE 3

#define PIPE_SC TEXT("\\\\.\\pipe\\testeSC") // do server, para o cliente (leitura/escrita)
#define PIPE_CS TEXT("\\\\.\\pipe\\testeCS") // do cliente, para o servidor (leitura/escrita)

//Tamanho do buffer de leitura
#define TAM 256
//Numero de clientes ligados em simultaneo ao server
#define N_PIPES 4


// HANDLES PARA OS PIPES 
//HANDLE hPipeRead[N_PIPES];
//HANDLE hPipeWrite[N_PIPES]; 

