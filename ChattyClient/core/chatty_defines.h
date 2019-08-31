#pragma once

#ifdef _WIN32
    #include <Windows.h>
    #define CLEAR_SCREEN system("cls")
#else
    #define CLEAR_SCREEN printf("\033c");
#endif

#define CHATTY_BUFFER_SIZE (1024 * 10) // 10KB
#define CHATTY_MESSAGE_SIZE (CHATTY_BUFFER_SIZE / sizeof(uint32_t))
