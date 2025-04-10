#pragma once

#include <common.h>

#define LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

#pragma comment(lib, "user32.lib")

struct Window {
    HINSTANCE instance = NULL;
    HWND handle = NULL;
    LPCSTR title = NULL;
    DWORD style = 0;
    int posX = 0;
    int posY = 0;
    int32 width = 0;
    int32 height = 0;
    bool fullscreen = false;
    bool vsync = false;
    RECT screen {0};
};

struct Display {
    int32 width = 0;
    int32 height = 0;
    DEVMODE mode {0};
};

class System {

    private:
        Window window;
        Display display;
        bool running = false;
            
    public:
        System(Settings);
        ~System();

        HWND GetWindowHandle();
        HINSTANCE GetInstance();
        bool IsRunning();
        void Update();
        
        static void SetDpiAwareness();
        static float GetTime();
        static void Delay(DWORD);
        static void CheckLastError();
        static string LoadTextFile(const char*);
        static void SetConsoleColor(WORD);
            
    private:
        static LRESULT CALLBACK WindowProc(HWND, uint32, WPARAM, LPARAM);
};

