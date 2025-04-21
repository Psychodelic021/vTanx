#pragma once

#include "window.h"

// wnd subsystem initialization
Window WindowInit(Config* cfg)
{
    PRINT_DEBUG("Window: initializing...\n");
    
    Window wnd = {0};

    // Set wnd properties
    wnd.title = cfg->name;
    wnd.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    wnd.instance = GetModuleHandle(0);
    wnd.fullscreen = cfg->fullscreen;
    wnd.vsync = cfg->vsync;
    wnd.isRunning = true;

    LPCSTR class_name = "ENGINE_wndClass";
    WNDCLASS wc = {
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = WindowProc,
        .hInstance = wnd.instance,
        .hCursor = LoadCursor(0, IDC_ARROW),
        .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
        .lpszClassName = class_name,
    };
    
    if (!RegisterClass(&wc))
    {
        PRINT_ERROR("Window error: registration failed\n");
        __debugbreak();
    }

    // Set DPI awareness context for the wnd
    SetDpiAwareness();

    wnd.dspWidth = GetSystemMetrics(SM_CXSCREEN);
    wnd.dspHeight = GetSystemMetrics(SM_CYSCREEN);

    if (wnd.fullscreen)
    {
        // Set the wnd style to fullscreen
        wnd.style = WS_POPUP | WS_VISIBLE;
        wnd.posX = 0;
        wnd.posY = 0;

        // Set the wnd size to the display size
        wnd.screen = (RECT){0, 0, wnd.dspWidth, wnd.dspHeight};
        AdjustWindowRect(&wnd.screen, wnd.style, 0);

        wnd.width = wnd.dspWidth;
        wnd.height = wnd.dspHeight;

        // Set the wnd style to fullscreen
        wnd.dspMode = (DEVMODE){
            .dmSize = sizeof(DEVMODE),
            .dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT,
            .dmBitsPerPel = 32,
            .dmPelsWidth = (DWORD)wnd.width,
            .dmPelsHeight = (DWORD)wnd.height
        };

        ChangeDisplaySettings(&wnd.dspMode, CDS_FULLSCREEN);
    }
    else
    {
        wnd.width = cfg->width;
        wnd.height = cfg->height;

        wnd.posX = (wnd.dspWidth - wnd.width) / 2;
        wnd.posY = (wnd.dspHeight - wnd.height) / 2;

        wnd.screen = (RECT){0, 0, wnd.width, wnd.height};
        AdjustWindowRect(&wnd.screen, wnd.style, 0);
    }

    wnd.handle = CreateWindow(class_name, wnd.title, wnd.style, wnd.posX, wnd.posY,
        wnd.screen.right - wnd.screen.left, wnd.screen.bottom - wnd.screen.top,
        0, 0, wnd.instance, 0);

    if (!wnd.handle) {
        PRINT_ERROR("Window error: Window creation failed");
        __debugbreak();
    }

    SetWindowLongPtr(wnd.handle, GWLP_USERDATA, (LONG_PTR)&wnd);

    ShowWindow(wnd.handle, SW_SHOW);
    SetFocus(wnd.handle);
    UpdateWindow(wnd.handle);

    PRINT_INFO("Window: ON\n");

    return wnd;
}

void WindowDestroy(Window* wnd)
{
    if ((wnd != NULL) && (wnd->handle != NULL)) {

        wnd->isRunning = false;

        // Destroy the wnd and set the handle to NULL
        DestroyWindow(wnd->handle);
        wnd->handle = NULL;
        // Unregister the wnd class before destroying the wnd
        UnregisterClass("ENGINE_wndClass", wnd->instance);

        *wnd = (Window){0}; // Reset the structure to zero
        wnd = NULL; // Set the pointer to NULL
    }
    
    PRINT_TRACE("Window: OFF\n");
}

// Window message pump
void WindowUpdate()
{
    MSG event = {0};
    while (PeekMessage(&event, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&event);
        DispatchMessage(&event);
    }
}

// Main window callback function
LRESULT CALLBACK WindowProc(HWND hwnd, uint32 message, WPARAM wparam, LPARAM lparam) {

    Window* wnd_ptr = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (message)
    {
        case WM_CLOSE: {
            DestroyWindow(hwnd);
        } break;

        case WM_DESTROY: {
            wnd_ptr->isRunning = false;
            // If the window is destroyed, we need to unregister the class
            
            PostQuitMessage(0);
        } break;
        
        case WM_SIZE: {
            if (wnd_ptr && wparam != SIZE_MINIMIZED) {
                // Update the window size
                wnd_ptr->width = LOWORD(lparam);
                wnd_ptr->height = HIWORD(lparam);
                
                // Set a flag to indicate the window has been resized
                if (wnd_ptr->width > 0 && wnd_ptr->height > 0) {
                    wnd_ptr->resized = true;
                }
            }
        } break;

        default: {
        } break;
    }

    return DefWindowProc(hwnd, message, wparam, lparam);
}

// Check if window was resized and reset the flag
bool WindowCheckResized(Window* wnd)
{
    if (wnd->resized) {
        wnd->resized = false;
        return true;
    }
    return false;
}


