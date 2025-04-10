#pragma once

#include <system.h>

// Window subsystem initialization
System::System(Settings settings)
{
    printf("System: initializing...\n");
    
    // Set window properties
    window.handle = NULL;
    window.title = settings.name;
    window.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    window.instance = GetModuleHandle(0);
    window.fullscreen = settings.fullscreen;
    window.vsync = settings.vsync;

    running = true;

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
        PRINT_ERROR("System ERROR: Window registration failed\n");
        throw std::exception();
    }

    // Set DPI awareness context for the window
    SetDpiAwareness();

    display.width = GetSystemMetrics(SM_CXSCREEN);
    display.height = GetSystemMetrics(SM_CYSCREEN);

    if (window.fullscreen)
    {
        // Set the window style to fullscreen
        window.style = WS_POPUP | WS_VISIBLE;
        window.posX = 0;
        window.posY = 0;

        // Set the window size to the display size
        window.screen = {0, 0, display.width, display.height};
        AdjustWindowRect(&window.screen, window.style, 0);

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
        window.width = settings.width;
        window.height = settings.height;

        window.posX = (display.width - window.width) / 2;
        window.posY = (display.height - window.height) / 2;

        window.screen = {0, 0, window.width, window.height};
        AdjustWindowRect(&window.screen, window.style, 0);
    }

    window.handle = CreateWindow(class_name, window.title, window.style, window.posX, window.posY,
        window.screen.right - window.screen.left, window.screen.bottom - window.screen.top,
        0, 0, window.instance, 0);

    if (!window.handle) {
        PRINT_ERROR("System ERROR: Window creation failed");
        throw std::exception();
    }

    SetWindowLongPtr(window.handle, GWLP_USERDATA, (LONG_PTR)this);

    ShowWindow(window.handle, SW_SHOW);
    SetFocus(window.handle);
    UpdateWindow(window.handle);

    printf("System: ON\n");
}

HWND System::GetWindowHandle()
{
    return window.handle;
}

HINSTANCE System::GetInstance()
{
    return window.instance;
}

bool System::IsRunning()
{
    return running;
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

    auto wnd_ptr = (System*)GetWindowLongPtr(wndhandle, GWLP_USERDATA);

    switch (message)
    {
        case WM_CLOSE: {
            DestroyWindow(wndhandle);
        } break;

        case WM_DESTROY: {
            wnd_ptr->running = false;
            // If the window is destroyed, we need to unregister the class
            PostQuitMessage(0);
        } break;

        default: {
        } break;
    }

    return DefWindowProc(wndhandle, message, wparam, lparam);
}

System::~System()
{
    running = false;

    if (window.handle)
    {   
        // Destroy the window and set the handle to NULL
        DestroyWindow(window.handle);
        window.handle = NULL;
        // Unregister the window class before destroying the window
        UnregisterClass("ENGINE_WindowClass", window.instance);
    }

    printf("System: OFF");
}

// Automatically choose the proper DPI awareness context based on the system DPI settings
void System::SetDpiAwareness()
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
        PRINT_ERROR("FormatMessage failed\n");
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
        std::printf("File %s: failed to get file size\n", filename);
        CheckLastError();
        return NULL;
    }

    char* buffer = new char[filesize.QuadPart + 1];
    if (!buffer) {
        PRINT_ERROR("Heap memory allocation failed\n");
        CheckLastError();
        return NULL;
    }

    DWORD bytesread = 0;
    result = ReadFile(file, buffer, (DWORD)filesize.QuadPart, &bytesread, NULL);
    if (!result || bytesread != filesize.QuadPart) {
        std::printf("File %s: failed to read\n", filename);
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

void System::SetConsoleColor(WORD color) {
    // Get the handle to the standard output
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        PRINT_ERROR("Failed to get console handle\n");
        return;
    }

    // Set the text color
    SetConsoleTextAttribute(hConsole, color);
}
