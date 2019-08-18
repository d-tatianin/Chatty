#pragma once

#ifdef _WIN32
	#include <Windows.h>
	#define CLEAR_SCREEN system("cls")
#else
	#define CLEAR_SCREEN printf("\033c");
#endif
