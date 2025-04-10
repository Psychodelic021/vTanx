#pragma once

#include <common.h>

#define LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

#pragma comment(lib, "user32.lib")

typedef struct Window {

    HINSTANCE instance;
    HWND handle;
    LPCSTR title;
    DWORD style;
    int posX, posY;
    uint width, height;
    bool isRunning;
    bool fullscreen;
    bool vsync;
    RECT screen;
    int dspWidth;
    int dspHeight;
    DEVMODE dspMode;
                
} Window;

Window WindowInit(Settings* set);

static LRESULT CALLBACK WindowProc(HWND, uint32, WPARAM, LPARAM);

HWND GetWindowHandle(Window* wnd);
HINSTANCE GetInstance(Window* wnd);

void SetDpiAwareness();
float GetTime();
void Delay(DWORD);
void CheckLastError();
char* LoadTextFile(const char*);
void SetConsoleColor(WORD);

