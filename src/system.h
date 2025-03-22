#pragma once

#include "types.h"

// API includes
#define LEAN_AND_MEAN
#include <Windows.h>
#include <Windowsx.h>

namespace System {

    // Window type
    struct Window {

        HINSTANCE instance;
        HWND handle;
        HDC devcon;
        HGLRC rndcon;
        const char* title;
        DWORD style;
        // Surface parameters
        int screen_width;
        int screen_height;
        int is_running;
        int monitor_width;
        int monitor_height;
        struct {
            int keys[256];
            int mouse_buttons[5];
            int mouse_x;
            int mouse_y;
        } input;

        // Constructor
        Window(int32 width, int32 height, const char* title);

        // Main window callback procedure
        static LRESULT CALLBACK Callback(HWND handle, UINT message, WPARAM w_param, LPARAM l_param);

        // Window message pump
        void update();
    };

    // System functions
    void CheckDpiAwareness();
    float GetTime();
    void CheckLastError();
    char* LoadTextFileToString(const char* filename);
    void DeallocateString(char* str);
    void SkipTime(int32 milliseconds);

};
