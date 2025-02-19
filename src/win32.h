#pragma once

// API includes
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

typedef struct {
    HINSTANCE instance;
    HWND handle;
    const char* title;
    UINT style;
    int width;
    int height;
    int is_running;
    MSG event;
} WindowState;

void Win32_WindowInit(WindowState* window, int width, int height, const char* title);
void Win32_WindowUpdate(WindowState* window);
