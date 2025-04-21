#pragma once

#include "system.h"

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

    PRINT_WARNING("%s\n", (char*)msgbuf);
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
        PRINT_ERROR("File %s: failed to read\n", filename);
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
