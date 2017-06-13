#pragma once
#include <tchar.h>
#include "CommonConstants.h"

typedef struct PipeMessage {

	TCHAR map[MAP_ROWS][MAP_COLUMNS];
	boolean snakeAlive;

}PipeMessage;