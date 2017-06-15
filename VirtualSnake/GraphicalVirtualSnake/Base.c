//Base.c
#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "..\DLL\CommonConstants.h"
#include "..\DLL\dll.h"
#include "Adapter.h"

/* ===================================================== */
/* Programa base (esqueleto) para aplica��es Windows     */
/* ===================================================== */
// Cria uma janela de nome "Janela Principal" e pinta fundo de branco
// Modelo para programas Windows:
//  Composto por 2 fun��es: 
//	WinMain()     = Ponto de entrada dos programas windows
//			1) Define, cria e mostra a janela
//			2) Loop de recep��o de mensagens provenientes do Windows
//     HandleEvents()= Processamentos da janela (pode ter outro nome)
//			1) � chamada pelo Windows (callback) 
//			2) Executa c�digo em fun��o da mensagem recebida
LRESULT CALLBACK HandleEvents(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK LocalGame(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK RemoteGame(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK DefineKeys(HWND, UINT, WPARAM, LPARAM);

// Nome da classe da janela (para programas de uma s� janela, normalmente este nome � 
// igual ao do pr�prio programa) "szprogName" � usado mais abaixo na defini��o das 
// propriedades do objecto janela

//GLOBAIS
TCHAR *szProgName = TEXT("VirtualSnake");
HINSTANCE hGlobalInst;
static TCHAR ip[100];
static TCHAR username[TAM];
static TCHAR up, left, right, down;
static boolean defaultKeys = TRUE;
TCHAR ** map;


// ============================================================================
// FUN��O DE IN�CIO DO PROGRAMA: WinMain()
// ============================================================================
// Em Windows, o programa come�a sempre a sua execu��o na fun��o WinMain()que desempenha // o papel da fun��o main() do C em modo consola WINAPI indica o "tipo da fun��o" (WINAPI // para todas as declaradas nos headers do Windows e CALLBACK para as fun��es de        // processamento da janela)
// Par�metros:
//   hInst: Gerado pelo Windows, � o handle (n�mero) da inst�ncia deste programa 
//   hPrevInst: Gerado pelo Windows, � sempre NULL para o NT (era usado no Windows 3.1)
//   lpCmdLine: Gerado pelo Windows, � um ponteiro para uma string terminada por 0
//              destinada a conter par�metros para o programa 
//   nCmdShow:  Par�metro que especifica o modo de exibi��o da janela (usado em  
//        	   ShowWindow()
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	HWND hWnd;		// hWnd � o handler da janela, gerado mais abaixo por CreateWindow()
	MSG lpMsg;		// MSG � uma estrutura definida no Windows para as mensagens
	WNDCLASSEX wcApp;	// WNDCLASSEX � uma estrutura cujos membros servem para 
	BOOL ret;			// definir as caracter�sticas da classe da janela
						// ============================================================================
						// 1. Defini��o das caracter�sticas da janela "wcApp" 
						//    (Valores dos elementos da estrutura "wcApp" do tipo WNDCLASSEX)
						// ============================================================================
	wcApp.cbSize = sizeof(WNDCLASSEX);	// Tamanho da estrutura WNDCLASSEX
	wcApp.hInstance = hInst;			// Inst�ncia da janela actualmente exibida
	hGlobalInst = hInst;
										// ("hInst" � par�metro de WinMain e vem 
										// inicializada da�)
	wcApp.lpszClassName = szProgName;	// Nome da janela (neste caso = nome do programa)
	wcApp.lpfnWndProc = HandleEvents;	// Endere�o da fun��o de processamento da janela 	// ("HandleEvents" foi declarada no in�cio e                 // encontra-se mais abaixo)
	wcApp.style = CS_HREDRAW | CS_VREDRAW;// Estilo da janela: Fazer o redraw se for      // modificada horizontal ou verticalmente
	
	wcApp.hIcon = LoadIcon(hInst, IDI_ICON1);
	//wcApp.hIcon = LoadIcon(NULL, IDI_APPLICATION);// "hIcon" = handler do �con normal
												  //"NULL" = Icon definido no Windows
												  // "IDI_AP..." �cone "aplica��o"
	wcApp.hIconSm = LoadIcon(hInst, IDI_ICON1);
	//wcApp.hIconSm = LoadIcon(NULL, IDI_INFORMATION);// "hIconSm" = handler do �con pequeno
													 //"NULL" = Icon definido no Windows
													 // "IDI_INF..." �con de informa��o
	wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);	// "hCursor" = handler do cursor (rato) 
													// "NULL" = Forma definida no Windows
													// "IDC_ARROW" Aspecto "seta" 
	wcApp.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	//wcApp.lpszMenuName = NULL;			// Classe do menu que a janela pode ter
										// (NULL = n�o tem menu)
	wcApp.cbClsExtra = 0;				// Livre, para uso particular
	wcApp.cbWndExtra = 0;				// Livre, para uso particular
	wcApp.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	// "hbrBackground" = handler para "brush" de pintura do fundo da janela. Devolvido por  // "GetStockObject".Neste caso o fundo ser� branco
	// ============================================================================
	// 2. Registar a classe "wcApp" no Windows
	// ============================================================================
	if (!RegisterClassEx(&wcApp))
		return(0);
	// ============================================================================
	// 3. Criar a janela
	// ============================================================================
	hWnd = CreateWindow(
		szProgName,			// Nome da janela (programa) definido acima
		TEXT("VirtualSnake"),// Texto que figura na barra do t�tulo
		WS_OVERLAPPEDWINDOW,	// Estilo da janela (WS_OVERLAPPED= normal)
		CW_USEDEFAULT,		// Posi��o x pixels (default=� direita da �ltima)
		CW_USEDEFAULT,		// Posi��o y pixels (default=abaixo da �ltima)
		CW_USEDEFAULT,		// Largura da janela (em pixels)
		CW_USEDEFAULT,		// Altura da janela (em pixels)
		(HWND)HWND_DESKTOP,	// handle da janela pai (se se criar uma a partir de
							// outra) ou HWND_DESKTOP se a janela for a primeira, 
							// criada a partir do "desktop"
		(HMENU)NULL,			// handle do menu da janela (se tiver menu)
		(HINSTANCE)hInst,		// handle da inst�ncia do programa actual ("hInst" � 
								// passado num dos par�metros de WinMain()
		0);				// N�o h� par�metros adicionais para a janela
						// ============================================================================
						// 4. Mostrar a janela
						// ============================================================================
	ShowWindow(hWnd, nCmdShow);	// "hWnd"= handler da janela, devolvido por 
								// "CreateWindow"; "nCmdShow"= modo de exibi��o (p.e. 
								// normal/modal); � passado como par�metro de WinMain()
	UpdateWindow(hWnd);		// Refrescar a janela (Windows envia � janela uma 
							// mensagem para pintar, mostrar dados, (refrescar)� 
							// ============================================================================
							// 5. Loop de Mensagens
							// ============================================================================
							// O Windows envia mensagens �s janelas (programas). Estas mensagens ficam numa fila de
							// espera at� que GetMessage(...) possa ler "a mensagem seguinte"	
							// Par�metros de "getMessage":
							// 1)"&lpMsg"=Endere�o de uma estrutura do tipo MSG ("MSG lpMsg" ja foi declarada no  
							//   in�cio de WinMain()):
							//			HWND hwnd		handler da janela a que se destina a mensagem
							//			UINT message		Identificador da mensagem
							//			WPARAM wParam		Par�metro, p.e. c�digo da tecla premida
							//			LPARAM lParam		Par�metro, p.e. se ALT tamb�m estava premida
							//			DWORD time		Hora a que a mensagem foi enviada pelo Windows
							//			POINT pt		Localiza��o do mouse (x, y) 
							// 2)handle da window para a qual se pretendem receber mensagens (=NULL se se pretendem //   receber as mensagens para todas as janelas pertencentes � thread actual)
							// 3)C�digo limite inferior das mensagens que se pretendem receber
							// 4)C�digo limite superior das mensagens que se pretendem receber

							// NOTA: GetMessage() devolve 0 quando for recebida a mensagem de fecho da janela,
							// 	  terminando ent�o o loop de recep��o de mensagens, e o programa 

	while ((ret = GetMessage(&lpMsg, NULL, 0, 0)) != 0) {
		if (ret != -1) {
			TranslateMessage(&lpMsg);	// Pr�-processamento da mensagem (p.e. obter c�digo 
										// ASCII da tecla premida)
			DispatchMessage(&lpMsg);	// Enviar a mensagem traduzida de volta ao Windows, que
										// aguarda at� que a possa reenviar � fun��o de 
										// tratamento da janela, CALLBACK HandleEvents (abaixo)
		}		
	}
	// ============================================================================
	// 6. Fim do programa
	// ============================================================================
	return((int)lpMsg.wParam);	// Retorna sempre o par�metro wParam da estrutura lpMsg
}
// ============================================================================
// FUN��O DE PROCESSAMENTO DA JANELA
// Esta fun��o pode ter um nome qualquer: Apenas � neces�rio que na inicializa��o da     // estrutura "wcApp", feita no in�cio de WinMain(), se identifique essa fun��o. Neste   // caso "wcApp.lpfnWndProc = WndProc"
//
// WndProc recebe as mensagens enviadas pelo Windows (depois de lidas e pr�-processadas // no loop "while" da fun��o WinMain()
//
// Par�metros:
//		hWnd	O handler da janela, obtido no CreateWindow()
//		messg	Ponteiro para a estrutura mensagem (ver estrutura em 5. Loop...
//		wParam	O par�metro wParam da estrutura messg (a mensagem)
//		lParam	O par�metro lParam desta mesma estrutura
//
// NOTA:Estes par�metros est�o aqui acess�veis o que simplifica o acesso aos seus valores
//
// A fun��o EndProc � sempre do tipo "switch..." com "cases" que descriminam a mensagem // recebida e a tratar. Estas mensagens s�o identificadas por constantes (p.e. 
// WM_DESTROY, WM_CHAR, WM_KEYDOWN, WM_PAINT...) definidas em windows.h ============================================================================
LRESULT CALLBACK HandleEvents(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {

	HDC device, auxDC;
	PAINTSTRUCT pt;
	static RECT map;
	static HBITMAP hBitmapFloor, hBitmapFood;
	static HBITMAP hBitmapSnakeHeadUp, hBitmapSnakeHeadDown, hBitmapSnakeHeadLeft, hBitmapSnakeHeadRight, hBitmapSnakeBodyHor, hBitmapSnakeBodyVer;
	static int x, y, alt, larg;

	switch (messg) {

	case WM_CREATE:

		if (!onCreate()) {
			MessageBox(hWnd, TEXT("Erro ao inicializar!"), TEXT("Erro"), MB_OK);
			exit(-1);
		}

		map.left = 0;
		map.top = 0;
		map.bottom = 300;
		map.right = 300;
		x = 0;
		y = 0; 
		alt = 108;
		larg = 68;

		hBitmapFloor = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP4));
		hBitmapFood = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP3));
		hBitmapSnakeHeadDown = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));
		hBitmapSnakeBodyHor = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2));
		hBitmapSnakeBodyVer = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP5));
		hBitmapSnakeHeadLeft = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP6));
		hBitmapSnakeHeadRight = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP7));
		hBitmapSnakeHeadUp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP8));

		
		break;

	case WM_LBUTTONDOWN:
		

		MessageBox(hWnd, up, TEXT("wrwerwer"), MB_YESNO);
		//InvalidateRect(hWnd, NULL, 0); //TODO: caso necessario, usar o RECT map 
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_CONTROLOS:
			//DialogBox(hGlobalInst, IDD_DIALOG_TECLAS, hWnd, DefineKeys);
			break;

		case ID_TIPODEJOGO_LOCAL:
			DialogBox(hGlobalInst, IDD_DIALOG_LOCAL, hWnd, LocalGame);
			break;

		case ID_TIPODEJOGO_REMOTO:
			DialogBox(hGlobalInst, IDD_DIALOG_REMOTO, hWnd, RemoteGame);
			break;

		case ID_SOBRE:
			DialogBox(hGlobalInst, IDD_DIALOG_SOBRE, hWnd, About);
			break;
		}
		break;

	case WM_PAINT: 
		device = BeginPaint(hWnd, &pt);

		auxDC = CreateCompatibleDC(device);
		
		SelectObject(auxDC, hBitmapFloor);

		for (int i = 0; i < MAP_ROWS; i++) {
			for (int j = 0; j < MAP_COLUMNS; j++) {
				BitBlt(device, 5 + 40 * j, 5 + 40*i, 40, 40, auxDC, 0, 0, SRCCOPY);
			}
		}

		SelectObject(auxDC, hBitmapFood);
		BitBlt(device, 5 + 40 * 5, 5 + 40 * 4, 40, 40, auxDC, 0, 0, SRCCOPY);
		
		SelectObject(auxDC, hBitmapSnakeHeadRight);
		BitBlt(device, 5 + 40 * 4, 5 + 40 * 4, 40, 40, auxDC, 0, 0, SRCCOPY);
		SelectObject(auxDC, hBitmapSnakeBodyHor);
		BitBlt(device, 5 + 40 * 3, 5 + 40 * 4, 40, 40, auxDC, 0, 0, SRCCOPY);
		BitBlt(device, 5 + 40 * 2, 5 + 40 * 4, 40, 40, auxDC, 0, 0, SRCCOPY);
		

		EndPaint(hWnd, &pt);
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, TEXT("Deseja fechar a janela?"), TEXT("Confirmar ac��o"), MB_YESNO) == IDYES) {
			PostQuitMessage(0);
		}
		break;

	case WM_DESTROY:	// Destruir a janela e terminar o programa 
						// "PostQuitMessage(Exit Status)"	
		DeleteObject(hBitmapFood);
		DeleteObject(hBitmapSnakeBodyHor);
		DeleteObject(hBitmapSnakeBodyVer);
		DeleteObject(hBitmapSnakeHeadUp);
		DeleteObject(hBitmapSnakeHeadLeft);
		DeleteObject(hBitmapSnakeHeadDown);
		DeleteObject(hBitmapSnakeHeadRight);
		DeleteObject(hBitmapFloor);
		PostQuitMessage(0);
		break;
	default:
		// Neste exemplo, para qualquer outra mensagem (p.e. "minimizar","maximizar","restaurar") // n�o � efectuado nenhum processamento, apenas se segue o "default" do Windows			
		return(DefWindowProc(hWnd,messg,wParam,lParam));		
	}
	return(0);
}

BOOL CALLBACK About(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {

	switch (messg) {

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

			case IDOK:
				EndDialog(hWnd, 0);
				return TRUE;
				break;
		
			case WM_CLOSE:
				EndDialog(hWnd, 0);
				return TRUE;
				break;

		}
		break;
	}

	return FALSE;
}

BOOL CALLBACK LocalGame(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {
	switch (messg) {

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case IDOK:
			GetDlgItemText(hWnd, IDC_EDIT1, username, TAM);

			if(_tcscmp(username, ""))
			{
				localGame();
				EndDialog(hWnd, 0);
				return TRUE;
			}
			break;

		case IDCANCEL:
			EndDialog(hWnd, 0);
			return FALSE;
			break;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return FALSE;
			break;
		}

		break;
	}

	return FALSE;

}

BOOL CALLBACK RemoteGame(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {

	switch (messg) {

	case WM_COMMAND:
		switch (LOWORD(wParam)) {

		case IDOK:
			GetDlgItemText(hWnd, IDC_EDIT1, ip, 100);
			EndDialog(hWnd, 0);
			return TRUE;
			break;

		case IDCANCEL:
			EndDialog(hWnd, 0);
			return TRUE;
			break;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;
			break;
		}

		break;
	}



	return FALSE;
}


//TODO: fix this 
BOOL CALLBACK DefineKeys(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {

	static boolean upPressed;
	static boolean downPressed;
	static boolean leftPressed;
	static boolean rightPressed;
	int total = 0;

	switch (messg) {

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_DOWN:
			down = _getch();
			//upPressed = TRUE;
			break;
		case IDC_BUTTON_UP:
			up = _getch();
			//downPressed = TRUE;
			break;
		case IDC_BUTTON_LEFT:
			left = _getch();
			//total++;
			break;
		case IDC_BUTTON_RIGHT:
			right = _getch();
			//total++;
			break;

		case IDOK:
			if (total < 4) {
				if (MessageBox(hWnd, TEXT("As teclas nao foram todas configuradas. Deseja continuar e usar as default?"), TEXT("Confirmar ac��o"), MB_YESNO) == IDYES) {
					defaultKeys = TRUE;
					EndDialog(hWnd, 0);
				}
			}
			else {

			}
		case IDCANCEL:
			if (total < 4) {
				if (MessageBox(hWnd, TEXT("As teclas nao foram todas configuradas. Deseja continuar e usar as default?"), TEXT("Confirmar ac��o"), MB_YESNO) == IDYES) {
					defaultKeys = TRUE;
					EndDialog(hWnd, 0);
				}
			}
		}
		break;

	case WM_CLOSE:
		if (total < 4) {
			if (MessageBox(hWnd, TEXT("As teclas nao foram todas configuradas. Deseja continuar e usar as default?"), TEXT("Confirmar ac��o"), MB_YESNO) == IDYES) {
				defaultKeys = TRUE;
				EndDialog(hWnd, 0);
			}
		
		}

	}
}






//LOGIC PART

boolean onCreate() {

	initSynchHandles();

	if (!WaitForSingleObject(hEventGameStarted, WAIT_TIMEOUT)) {
		_tprintf(TEXT("J� existe um jogo a decorrer! A aplica��o vai fechar"));
		_tprintf(TEXT("\n\nPressione uma tecla para continuar"));
		_getch();
		return FALSE;
	}

	map = initMalloc();

	return TRUE;
}


TCHAR ** initMalloc() {

	TCHAR ** map = (TCHAR **)malloc(MAP_ROWS * sizeof(TCHAR *));
	if (map == NULL) {
		//tcout << TEXT("Erro na alocacao de memoria de TCHAR** (map) !");
		_tprintf(TEXT("Erro na alocacao de memoria de TCHAR** (map) !"));
		exit(0);
	}

	for (int i = 0; i < MAP_ROWS; i++) {
		map[i] = (TCHAR *)malloc(MAP_COLUMNS * sizeof(TCHAR));
		if (map == NULL) {
			//tcout << TEXT("Erro na alocacao de memoria de TCHAR* (map) !");
			_tprintf(TEXT("Erro na alocacao de memoria de TCHAR* (map) !"));
			exit(0);
		}
	}

	return map;
}


boolean localGame() {

	//	TCHAR up, down, left, right;
	//TCHAR res;

	initMemory();


	if (getOwnKeyArrayPosition() == -1) {
		return FALSE;
	}

	setUsername(username);

	//TCHAR *teclas = chooseKeys();

	WaitForSingleObject(hEventGameStarted, INFINITE);

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WaitForMapChanges, (LPVOID)map, 0, NULL);

	/*while (1) {

	TCHAR aux = keyPress(teclas);

	if (aux != ' ') {
	newKeyPressed(aux);
	}

	}*/
}

DWORD WINAPI WaitForMapChanges(LPVOID param) {

	TCHAR ** map = (TCHAR **)param;
	boolean alive = TRUE;

	while (readMapInMemory(map)) {
		//tcout << TEXT("Vou ler!") << endl;
		//alive = ;
		//tcout << TEXT("Ja li!") << endl;
		//system("cls");		//clrscr();

		for (int i = 0; i < MAP_ROWS; i++) {
			for (int j = 0; j < MAP_COLUMNS; j++) {
				//tcout << map[i][j];
				//_tprintf(TEXT("%c"), map[i][j]);
				//MessageBox()
			}
			//tcout << endl;
			_tprintf(TEXT("\n"));
		}
	}

	//tcout << TEXT("Morri! apresenta o score!");
	_tprintf(TEXT("Morri! apresenta o score!"));

	return TRUE;


}