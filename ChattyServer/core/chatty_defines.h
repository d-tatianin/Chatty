#pragma once

#ifdef _WIN32
    #include <Windows.h>
    inline void clear_screen()
    {
        system("cls");
    }
#else
    #include <stdio.h>
    inline void clear_screen()
    {
        printf("\033c");
    }
#endif

#define CHATTY_BUFFER_SIZE (1024 * 10) // 10KB
#define CHATTY_MESSAGE_SIZE (CHATTY_BUFFER_SIZE / sizeof(uint32_t))
