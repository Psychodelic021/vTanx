#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

typedef struct {
    HWND handle;
    int width;
    int height;
    const char* title;
    int is_running;
} WindowState;

LRESULT CALLBACK Win32_WindowCallback(HWND handle, UINT message, WPARAM w_param, LPARAM l_param) {
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

void Win32_WindowInit(WindowState* window, int width, int height, const char* title) {

    window->width = width;
    window->height = height;
    window->title = title;
    window->is_running = 1;

    char* class_name = "vTanxWindowClass";
    HINSTANCE instance = GetModuleHandle(0);

    WNDCLASS wc = {
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = Win32_WindowCallback,
        .hInstance = instance,
        .lpszClassName = class_name,
        .hCursor = LoadCursor(0, IDC_ARROW),
        .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
    };
    
    RegisterClass(&wc);
    window->handle = CreateWindow(class_name, window->title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
        window->width, window->height, 0, 0, instance, 0);

}