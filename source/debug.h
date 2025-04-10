#pragma once

#define PRINT_INFO(msg) { \
    SetConsoleColor(FOREGROUND_GREEN); \
    printf(msg); \
    SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
}

#define PRINT_DEBUG(msg) { \
    SetConsoleColor(FOREGROUND_BLUE); \
    printf(msg); \
    SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
}

#define PRINT_ERROR(msg) { \
    SetConsoleColor(FOREGROUND_RED); \
    printf(msg); \
    SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
}
