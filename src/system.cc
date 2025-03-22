#include "system.h"

namespace System {

    // Main window callback function
    LRESULT CALLBACK Window::Callback(HWND handle, UINT message, WPARAM w_param, LPARAM l_param) {
        
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
    Window::Window(int32 width, int32 height, const char* title) {

        this->title = title;
        screen_width = width;
        screen_height = height;
        style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
        is_running = 1;
        instance = GetModuleHandle(0);

        const char* class_name = "vTanxWindowClass";
        WNDCLASS wc = {
            .style = CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc = Callback,
            .hInstance = instance,
            .hCursor = LoadCursor(0, IDC_ARROW),
            .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
            .lpszClassName = class_name,
        };
        
        if (!RegisterClass(&wc)) {
            MessageBox(0, "Window registration failed", "Error", MB_ICONERROR);
            __debugbreak();
        }

        // Automatically choose the proper DPI awareness context
        if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) {
            printf("Failed to set DPI awareness context to PER_MONITOR_AWARE_V2\n");
            if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE)) {
                printf("Failed to set DPI awareness context to PER_MONITOR_AWARE\n");
                if (!SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE)) {
                    printf("Failed to set DPI awareness context to SYSTEM_AWARE\n");
                    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_UNAWARE);
                    printf("Set DPI awareness context to UNAWARE\n");
                }
            }
        }

        // Check and print the current DPI awareness context
        CheckDpiAwareness();

        monitor_width = GetSystemMetrics(SM_CXSCREEN);
        monitor_height = GetSystemMetrics(SM_CYSCREEN);

        int pos_x = (monitor_width - width) / 2;
        int pos_y = (monitor_height - height) / 2;

        RECT screen = {0, 0, screen_width, screen_height};
        AdjustWindowRect(&screen, style, 0);

        handle = CreateWindow(class_name, title,
            style, pos_x, pos_y,
            screen.right - screen.left, screen.bottom - screen.top,
            0, 0, instance, 0);

        if (!handle) {
            MessageBox(0, "Window creation failed", "Error", MB_ICONERROR);
            __debugbreak();
        }

        devcon = GetDC(handle);
        rndcon = NULL;

        ShowWindow(handle, SW_SHOW);
        UpdateWindow(handle);
    }

    // Window message pump
    void Window::update() {

        MSG event = {0};
        while (PeekMessage(&event, 0, 0, 0, PM_REMOVE)) {
            
            if (event.message == WM_QUIT) {
               is_running = 0;
            }
            
            switch (event.message) {

                case WM_KEYDOWN: {
                    input.keys[event.wParam] = 1;
                } break;
        
                case WM_KEYUP: {
                    input.keys[event.wParam] = 0;
                } break;
        
                case WM_LBUTTONDOWN: {
                    input.mouse_buttons[0] = 1;
                } break;
        
                case WM_LBUTTONUP: {
                    input.mouse_buttons[0] = 0;
                } break;
        
                case WM_RBUTTONDOWN: {
                    input.mouse_buttons[1] = 1;
                } break;
        
                case WM_RBUTTONUP: {
                    input.mouse_buttons[1] = 0;
                } break;
        
                case WM_MOUSEMOVE: {
                    input.mouse_x = GET_X_LPARAM(event.lParam);
                    input.mouse_y = GET_Y_LPARAM(event.lParam);
                } break;
        
                default: {
                    TranslateMessage(&event);
                    DispatchMessage(&event);
                } break;
            }
        }
    }

    // Function to check and print the current DPI awareness context
    void CheckDpiAwareness()
    {
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
            printf("FormatMessage failed\n");
            return;
        }

        printf("%s\n", (LPCTSTR)msgbuf);
        LocalFree(msgbuf);
    }

    // Load a text file into a string
    char* LoadTextFileToString(const char* filename)
    {
        HANDLE file = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file == INVALID_HANDLE_VALUE) {
            printf("File %s: failed to create a file handle\n", filename);
            return NULL;
        }

        LARGE_INTEGER filesize = {0};
        BOOL result = GetFileSizeEx(file, &filesize);
        if (!result) {
            printf("File %s: failed to get file size\n", filename);
            CheckLastError();
            return NULL;
        }

        char* buffer = (char*)malloc(filesize.QuadPart + 1);
        if (!buffer) {
            printf("Heap memory allocation failed\n");
            CheckLastError();
            return NULL;
        }
        else {
            printf("Heap memory allocated: %lld bytes\n", filesize.QuadPart + 1);
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

    // Deallocate a string
    void DeallocateString(char* str)
    {
        free(str);
        str = NULL;
    }

    void SkipTime(int32 milliseconds)
    {
        Sleep(milliseconds);
    }
}