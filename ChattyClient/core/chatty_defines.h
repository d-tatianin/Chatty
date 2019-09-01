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

#ifdef _WIN32
    #include <io.h>
    #include <fcntl.h>

    inline void init_sink()
    {
        _setmode(_fileno(stdout), _O_U16TEXT);
        _setmode(_fileno(stdin), _O_U16TEXT);
        _setmode(_fileno(stderr), _O_U16TEXT);
    }
#else
    #include <locale>

    inline void init_sink()
    {
        setlocale(LC_ALL, "en_US.utf8");
    }
#endif
