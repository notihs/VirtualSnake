#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "Main.h"
#include "..\DLL\dll.h"

using namespace std;

#define BUFFER 256

int _tmain(int argc, TCHAR *argv[]) {
	
	#ifdef UNICODE
	#define tcout wcout
	#define tcin wcin
	#define tstring wstring

	#else
	#define tcout cout
	#define tcin cin
	#define tstring string
	#endif

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
	_setmode(_fileno(stderr), _O_WTEXT);
#endif

	TCHAR tecla[BUFFER];
	TCHAR user[BUFFER];
	bool stayHere;

	_tprintf(TEXT("Bem-vindo ao Virtual Snake! Escolha o seu username! \n->"));
	_fgetts(user, BUFFER, stdin); //NOTA: username vai com \n no fim

	do {
		stayHere = false;

		_tprintf(TEXT("\nEscolha o tipo de jogo!"));
		_tprintf(TEXT("\n1 - Jogo Local"));
		_tprintf(TEXT("\n2 - Jogo Remoto"));
		_tprintf(TEXT("\n3 - Sair  \n->"));
		_fgetts(tecla, BUFFER, stdin);

		_tprintf(TEXT("\n"));

		if (!_tcscmp(tecla, TEXT("1\n"))) {
			_tprintf(TEXT("Aguarde pelo Jogo Local!\n\n")); //TODO: vai buscar o mapa a DLL
			readMapInMemory(); //DLL
		}
		else if (!_tcscmp(tecla, TEXT("2\n"))) {
			_tprintf(TEXT("Aguarde pelo Jogo Remoto!")); //TODO: configurar namedPipe
		}
		else if (!_tcscmp(tecla, TEXT("3\n"))) {
			_tprintf(TEXT("Obrigado por jogar!"));
			return 0;
		}
		else {
			_tprintf(TEXT("Escolha uma opção válida!\n"));
			stayHere = true;
		}

	} while (stayHere);



	return 0;
}

