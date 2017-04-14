#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

void defines();
bool isValidInt(int);

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

	tstring x = TEXT("");

	defines();

	//TODO: verificar se o server esta on e se é o primeiro cliente!
	do {

		tcout << TEXT("Insert the maximum number of players: ");
		tcin >> x;
		

	} while (true);

	//_tprintf(TEXT("Result: %d"), num);

	return 0;
}

void defines(){
	#ifdef UNICODE
		_setmode(_fileno(stdin), _O_WTEXT);
		_setmode(_fileno(stdout), _O_WTEXT);
		_setmode(_fileno(stderr), _O_WTEXT);
#endif


}

bool isValidInt(int x) {

	if (x == 1) {
		return true;
	}

	return false;
}