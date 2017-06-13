#include "Utils.h"

TCHAR* chooseKeys() {

	TCHAR up, down, left, right;
	TCHAR res;

	do {
		//tcout << TEXT("Escolha as teclas para se movimentar: ");
		_tprintf(TEXT("Escolha as teclas para se movimentar: "));
		//tcout << TEXT("\n\nTecla para cima: ");
		_tprintf(TEXT("\n\nTecla para cima: "));
		up = _getch();
		do {
			//tcout << TEXT("\nTecla para baixo: ");
			_tprintf(TEXT("\nTecla para baixo: "));
			down = _getch();
		} while (down == up);
		do {
			//tcout << TEXT("\nTecla para esquerda: ");
			_tprintf(TEXT("\nTecla para esquerda: "));
			left = _getch();
		} while (left == down || left == up);
		do {
			//tcout << TEXT("\nTecla para direita: ");
			_tprintf(TEXT("\nTecla para direita: "));
			right = _getch();
		} while (right == down || right == up || right == left);

		_tprintf(TEXT("\n\nTeclas escolhidas: "));
		_tprintf(TEXT("\nCima: %c"), up);
		_tprintf(TEXT("\nBaixo: %c"), down);
		_tprintf(TEXT("\nEsquerda: %c"), left);
		_tprintf(TEXT("\nDireita: %c"), right);
		_tprintf(TEXT("\n\nConfirma estas teclas? (S/N)"));
		/*
		tcout << TEXT("\n\nTeclas escolhidas: ");
		tcout << endl << TEXT("Cima: ") << up;
		tcout << endl << TEXT("Baixo: ") << down;
		tcout << endl << TEXT("Esquerda: ") << left;
		tcout << endl << TEXT("Direita: ") << right;
		tcout << endl << endl << TEXT("Confirma estas teclas? (S/N)");*/
		res = _getch();

	} while (_totupper(res) != 'S');

	TCHAR * teclas = (TCHAR *) malloc(sizeof(TCHAR) * 4);
	//teclas = { up, down, left, right };
	teclas[0] = up;
	teclas[1] = down;
	teclas[2] = left;
	teclas[3] = right;

	return teclas;
}

TCHAR keyPress(TCHAR *teclas) {

	TCHAR up = teclas[0];
	TCHAR down = teclas[1];
	TCHAR left = teclas[2];
	TCHAR right = teclas[3];


	//tcout << endl << TEXT("Movimento:");
	TCHAR key = _getch();
	TCHAR aux;

	if (key == up) {
		aux = 'u';
	}
	else if (key == down) {
		aux = 'd';
	}
	else if (key == left) {
		aux = 'l';
	}
	else if (key == right) {
		aux = 'r';
	}
	else {
		aux = ' ';
	}

	return aux;
}
