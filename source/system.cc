#pragma once

#include <iostream>

#include <system.h>

// Window subsystem initialization
void System::Init(SystemSettings sysset)
{
    std::cout << "System: Initializing..." << std::endl;

    // Set window properties
    window.handle = NULL;
    window.title = sysset.name;
    window.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    window.instance = GetModuleHandle(0);
    window.fullscreen = sysset.fullscreen;
    window.vsync = sysset.vsync;

    LPCSTR class_name = "ENGINE_WindowClass";
    WNDCLASS wc = {
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = WindowProc,
        .hInstance = window.instance,
        .hCursor = LoadCursor(0, IDC_ARROW),
        .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
        .lpszClassName = class_name,
    };
    
    if (!RegisterClass(&wc))
    {
        std::cerr << "System error: Window registration failed" << std::endl;
        throw std::runtime_error("System error: Window registration failed");
    }

    // Set DPI awareness context for the window
    SetDpiAwareness();

    display.width = GetSystemMetrics(SM_CXSCREEN);
    display.height = GetSystemMetrics(SM_CYSCREEN);

    if (window.fullscreen)
    {
        window.width = display.width;
        window.height = display.height;

        // Set the window style to fullscreen
        display.mode = {
            .dmSize = sizeof(DEVMODE),
            .dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT,
            .dmBitsPerPel = 32,
            .dmPelsWidth = (DWORD)window.width,
            .dmPelsHeight = (DWORD)window.height
        };

        ChangeDisplaySettings(&display.mode, CDS_FULLSCREEN);
    }
    else
    {
        window.posX = (display.width - sysset.width) / 2;
        window.posY = (display.height - sysset.height) / 2;

        window.screen = {0, 0, window.width, window.height};
        AdjustWindowRect(&window.screen, window.style, 0);
    }

    window.handle = CreateWindow(class_name, window.title, window.style, window.posX, window.posY,
        window.screen.right - window.screen.left, window.screen.bottom - window.screen.top,
        0, 0, window.instance, 0);

    if (!window.handle) {
        std::cerr << "Window creation failed" << std::endl;
        throw std::runtime_error("Window creation failed");
    }

    ShowWindow(window.handle, SW_SHOW);
    SetFocus(window.handle);
    UpdateWindow(window.handle);

    std::cout << "System: OK" << std::endl;
}

HWND System::GetWindowHandle()
{
    return HWND();
}

// Window message pump
void System::Update()
{
    MSG event {0};
    while (PeekMessage(&event, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&event);
        DispatchMessage(&event);
    }
}

// Main window callback function
LRESULT CALLBACK System::WindowProc(HWND wndhandle, uint32 message, WPARAM wparam, LPARAM lparam) {

    switch (message)
    {
        case WM_CLOSE: {
            DestroyWindow(wndhandle);
        } break;

        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;

        default: {
        } break;
    }

    return DefWindowProc(wndhandle, message, wparam, lparam);
}

void System::Destroy()
{
    if (window.handle)
    {   
        // Unregister the window class before destroying the window
        UnregisterClass("ENGINE_WindowClass", window.instance);
        // Destroy the window and set the handle to NULL
        DestroyWindow(window.handle);
        window.handle = NULL;
    }

    std::cout << "System: Shutting down..." << std::endl;
}

// Destructor for the System class.
System::~System()
{
    Destroy();
}

// Automatically choose the proper DPI awareness context based on the system DPI settings
void System::SetDpiAwareness()
{

    if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {
        std::cerr << "Failed to set DPI awareness context to PER_MONITOR_AWARE_V2\n";
        if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE)) {
            std::cerr << "Failed to set DPI awareness context to PER_MONITOR_AWARE\n";
            if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE)) {
                std::cerr << "Failed to set DPI awareness context to SYSTEM_AWARE\n";
                SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE);
                std::cout << "Set DPI awareness context to UNAWARE\n";
            }
        }

        std::cout << std::endl;
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

    std::cout << "Current DPI Awareness: " << awarenessString << std::endl;
}

// Get current time in seconds
float System::GetTime()
{
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return ((float)counter.QuadPart / (float)frequency.QuadPart);
}

void System::Delay(DWORD milliseconds)
{
    Sleep(milliseconds);
}

// Check for the last error and print it
void System::CheckLastError()
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
        std::cout << "FormatMessage failed\n" << std::endl;
        return;
    }

    std::cout << (const char*)msgbuf << std::endl;
    LocalFree(msgbuf);
}

// Load a text file into a string
string System::LoadTextFile(const char* filename)
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

    char* buffer = new char[filesize.QuadPart + 1];
    if (!buffer) {
        printf("Heap memory allocation failed\n");
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

    string str(buffer); // Convert to std::string for easier handling
    delete buffer; // Free the buffer after copying to string
    return str;
}
