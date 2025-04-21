#include "system.h"
#include "debug.h"
#include <fstream>
#include <sstream>

namespace System {

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
        case DPI_AWARENESS_INVALID:
            awarenessString = "DPI_AWARENESS_INVALID";
            break;
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
    return static_cast<float>(counter.QuadPart) / static_cast<float>(frequency.QuadPart);
}

void Delay(DWORD milliseconds)
{
    Sleep(milliseconds);
}

// Check for the last error and print it
void CheckLastError()
{
    LPVOID msgbuf = nullptr;
    DWORD error = GetLastError();

    if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPTSTR>(&msgbuf),
        0, NULL))
    {
        PRINT_ERROR("FormatMessage failed\n");
        return;
    }

    PRINT_WARNING("%s\n", static_cast<char*>(msgbuf));
    LocalFree(msgbuf);
}

// Load a text file into a string using C++ file I/O
std::string LoadTextFile(const char* filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        PRINT_ERROR("File %s: failed to open\n", filename);
        return std::string();
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    return buffer.str();
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

} // namespace System