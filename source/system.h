#pragma once

// API includes
#define LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

#include "types.h"

using std::string;

namespace System {
    
    struct Input {
        int keys[256];
        int mouse_buttons[5];
        int mouse_x;
        int mouse_y;
    };

    struct Screen {
        int width;
        int height;
        int display_width;
        int display_height;
    };

    class Window {
    public:
        // Window properties
        HINSTANCE instance = NULL;
        HWND handle = NULL;
        LPCSTR title = NULL;
        DWORD style = 0;
        // Window state
        bool is_running = FALSE;
        // Subsystem properties
        Screen screen = { 0 };
        Input input = { 0 };

        // Constructor
        Window() = default;
        ~Window();
        Window(const Window&) = delete;
        Window operator=(const Window&) = delete;

        // Methods
        bool Init(int32, int32, const char*);
        void Update();
        void Shutdown();

        // Main window callback procedure
        static LRESULT CALLBACK CallbackFunction(HWND, UINT, WPARAM, LPARAM);
    };

    // System functions
    void SetDpiAwareness();
    void GetDpiAwareness();
    float GetTime();
    void CheckLastError();
    string LoadTextFile(const char*);
    void SkipTime(int32);
}