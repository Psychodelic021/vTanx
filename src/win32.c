#include "win32.h"

// Main window callback function
static LRESULT CALLBACK Win32_WindowCallback(HWND handle, UINT message, WPARAM w_param, LPARAM l_param) {
    
    LRESULT result = 0;

    switch (message) {
        case WM_CLOSE: {
            DestroyWindow(handle);
        } break;

        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;

        default: {
            result = DefWindowProc(handle, message, w_param, l_param);
        } break;
    }

    return result;
}

// Window subsystem initialization
void Win32_WindowInit(WindowState* window, int width, int height, const char* title) {

    window->width = width;
    window->height = height;
    window->title = title;
    window->style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    window->is_running = 1;
    window->instance = GetModuleHandle(0);
    memset(&window->event, 0, sizeof(window->event));

    char* class_name = "vTanxWindowClass";
    WNDCLASS wc = {
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = Win32_WindowCallback,
        .hInstance = window->instance,
        .lpszClassName = class_name,
        .hCursor = LoadCursor(0, IDC_ARROW),
        .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
    };
    
    if (!RegisterClass(&wc)) {
        MessageBox(0, "Window registration failed", "Error", MB_ICONERROR);
        return;
    }

    RECT screen = {0, 0, window->width, window->height};
    AdjustWindowRect(&screen, window->style, 0);

    window->handle = CreateWindow(class_name, window->title,
        window->style, CW_USEDEFAULT, CW_USEDEFAULT,
        screen.right - screen.left, screen.bottom - screen.top,
        0, 0, window->instance, 0);

    if (!window->handle) {
        MessageBox(0, "Window creation failed", "Error", MB_ICONERROR);
        return;
    }

    ShowWindow(window->handle, SW_SHOW);
    UpdateWindow(window->handle);
}

// Window message pump
void Win32_WindowUpdate(WindowState* window) {
    MSG message = {0};
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
        
        if (message.message == WM_QUIT) {
            window->is_running = 0;
        }
        
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}