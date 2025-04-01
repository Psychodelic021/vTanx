#pragma once

#define LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

#include <common.h>

struct SystemSettings {
    int width;
    int height;
    const char* name;
    bool fullscreen;
    bool vsync;
};

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
        
    public:
    System() = default;
    ~System();
    System(const System&) = delete;
    System operator= (const System&) = delete;

    void Init(SystemSettings);
    void Destroy();

    HWND GetWindowHandle();
    void Update();

    void SetDpiAwareness();
    float GetTime();
    void Delay(DWORD);
    void CheckLastError();
    string LoadTextFile(const char*);
        
    private:
    static LRESULT CALLBACK WindowProc(HWND, uint32, WPARAM, LPARAM);
};

