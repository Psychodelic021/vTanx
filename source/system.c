#pragma once

#include <system.h>

// wnd subsystem initialization
Window WindowInit(Settings* set)
{
    printf("System: initializing...\n");
    
    Window wnd = {0};

    // Set wnd properties
    wnd.title = set->name;
    wnd.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    wnd.instance = GetModuleHandle(0);
    wnd.fullscreen = set->fullscreen;
    wnd.vsync = set->vsync;
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
        PRINT_ERROR("System ERROR: wnd registration failed\n");
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
        AdjustwndRect(&wnd.screen, wnd.style, 0);

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
        wnd.width = set->width;
        wnd.height = set->height;

        wnd.posX = (wnd.dspWidth - wnd.width) / 2;
        wnd.posY = (wnd.dspHeight - wnd.height) / 2;

        wnd.screen = (RECT){0, 0, wnd.width, wnd.height};
        AdjustwndRect(&wnd.screen, wnd.style, 0);
    }

    wnd.handle = Createwnd(class_name, wnd.title, wnd.style, wnd.posX, wnd.posY,
        wnd.screen.right - wnd.screen.left, wnd.screen.bottom - wnd.screen.top,
        0, 0, wnd.instance, 0);

    if (!wnd.handle) {
        PRINT_ERROR("System ERROR: wnd creation failed");
        __debugbreak();
    }

    SetWindowLongPtr(wnd.handle, GWLP_USERDATA, &wnd);

    ShowWindow(wnd.handle, SW_SHOW);
    SetFocus(wnd.handle);
    UpdateWindow(wnd.handle);

    printf("System: ON\n");

    return wnd;
}

HWND Gethwnd(Window* wnd)
{
    return wnd->handle;
}

HINSTANCE GetInstance(Window* wnd)
{
    return wnd->instance;
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

        default: {
        } break;
    }

    return DefWindowProc(hwnd, message, wparam, lparam);
}

void WindowDestroy(Window* wnd)
{
    wnd->isRunning = false;

    if (wnd->handle)
    {   
        // Destroy the wnd and set the handle to NULL
        DestroyWindow(wnd->handle);
        wnd->handle = NULL;
        // Unregister the wnd class before destroying the wnd
        UnregisterClass("ENGINE_wndClass", wnd->instance);
    }

    printf("Window: OFF");
}

// Automatically choose the proper DPI awareness context based on the system DPI settings
void SetDpiAwareness()
{

    if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {
        PRINT_ERROR("Failed to set DPI awareness context to PER_MONITOR_AWARE_V2\n");
        if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE)) {
            PRINT_ERROR("Failed to set DPI awareness context to PER_MONITOR_AWARE\n");
            if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE)) {
                PRINT_ERROR("Failed to set DPI awareness context to SYSTEM_AWARE\n");
                SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE);
                printf("Set DPI awareness context to UNAWARE\n");
            }
        }
        
        printf("\n");
    }

    DPI_AWARENESS_CONTEXT dpiContext = GetThreadDpiAwarenessContext();
    DPI_AWARENESS dpiAwareness = GetAwarenessFromDpiAwarenessContext(dpiContext);

    const char* awarenessString = "Unknown";
    switch (dpiAwareness)
    {
        case DPI_AWARENESS_UNAWARE:
            awarenessString = "DPI_AWARENESS_UNAWARE";
            break;
        case DPI_AWARENESS_SYSTEM_AWARE:
            awarenessString = "DPI_AWARENESS_SYSTEM_AWARE";
            break;
        case DPI_AWARENESS_PER_MONITOR_AWARE:
            awarenessString = "DPI_AWARENESS_PER_MONITOR_AWARE";
            break;
    }

    printf("Current DPI Awareness: %s\n", awarenessString);
}

// Get current time in seconds
float GetTime()
{
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return ((float)counter.QuadPart / (float)frequency.QuadPart);
}

void Delay(DWORD milliseconds)
{
    Sleep(milliseconds);
}

// Check for the last error and print it
void CheckLastError()
{
    void* msgbuf = NULL;
    DWORD error = GetLastError();

    if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&msgbuf,
        0, NULL))
    {
        PRINT_ERROR("FormatMessage failed\n");
        return;
    }

    printf("%s\n", msgbuf);
    LocalFree(msgbuf);
}

// Load a text file into a string
char* LoadTextFile(const char* filename)
{
    HANDLE file = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) {
        printf("File %s: failed to create a file handle\n", filename);
        return NULL;
    }

    LARGE_INTEGER filesize = {0};
    bool result = GetFileSizeEx(file, &filesize);
    if (!result) {
        printf("File %s: failed to get file size\n", filename);
        CheckLastError();
        return NULL;
    }

    char* buffer = malloc((size_t)filesize.QuadPart + 1);
    if (!buffer) {
        PRINT_ERROR("Heap memory allocation failed\n");
        CheckLastError();
        return NULL;
    }

    DWORD bytesread = 0;
    result = ReadFile(file, buffer, (DWORD)filesize.QuadPart, &bytesread, NULL);
    if (!result || bytesread != filesize.QuadPart) {
        printf("File %s: failed to read\n", filename);
        CheckLastError();
        free(buffer); // Free allocated memory in case of failure
        buffer = NULL;
        return NULL;
    }

    CloseHandle(file);

    buffer[filesize.QuadPart] = '\0'; // Null-terminate the buffer

    return buffer;
}

void SetConsoleColor(WORD color) {
    // Get the handle to the standard output
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        PRINT_ERROR("Failed to get console handle\n");
        return;
    }

    // Set the text color
    SetConsoleTextAttribute(hConsole, color);
}
