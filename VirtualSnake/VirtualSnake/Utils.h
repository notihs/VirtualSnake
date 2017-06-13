#pragma once

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
//#include <iostream>
//#include <string>
#include <conio.h>
#include "..\DLL\CommonConstants.h"
#include "..\DLL\dll.h"

TCHAR * chooseKeys();
TCHAR keyPress(TCHAR *teclas);