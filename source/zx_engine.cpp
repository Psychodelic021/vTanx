// Single Translation Unit Build - C++ Version
// C++ Main Implementation File

// Core headers first
#include "common.h"
#include "config.h"
#include "debug.h"
#include "system.h"
#include "vmath.h"
#include "vulkan.h"

// Implementations for window system
#define IMPLEMENTATION
#include "window.cpp"

// C implementation code - include directly for STU compilation
// but without extern "C" since vmath.h contains C++ classes
#include "vmath.c"
#include "vulkan.c"

// Implementation of system utilities
namespace System {
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
    std::string LoadTextFile(const char* filename)
    {
        std::string result;
        HANDLE file = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file == INVALID_HANDLE_VALUE) {
            printf("File %s: failed to create a file handle\n", filename);
            return result;
        }

        LARGE_INTEGER filesize = {0};
        bool success = GetFileSizeEx(file, &filesize);
        if (!success) {
            printf("File %s: failed to get file size\n", filename);
            CheckLastError();
            return result;
        }

        // Allocate memory for the file content
        char* buffer = new char[(size_t)filesize.QuadPart + 1];
        if (!buffer) {
            PRINT_ERROR("Memory allocation failed\n");
            CheckLastError();
            return result;
        }

        DWORD bytesread = 0;
        success = ReadFile(file, buffer, (DWORD)filesize.QuadPart, &bytesread, NULL);
        if (!success || bytesread != filesize.QuadPart) {
            PRINT_ERROR("File %s: failed to read\n", filename);
            CheckLastError();
            delete[] buffer;
            return result;
        }

        CloseHandle(file);

        buffer[filesize.QuadPart] = '\0'; // Null-terminate the buffer
        result = buffer; // Convert to std::string
        delete[] buffer;  // Clean up the buffer

        return result;
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

// Entry point
int main()
{
    // Create configuration with default values
    ZX::Config cfg;
    
    // Create window using the modernized Window class
    auto window = ZX::Window::Create(cfg);
    if (!window) {
        PRINT_ERROR("Failed to create window\n");
        return -1;
    }
    
    // Set up window event callback for notifications
    window->SetEventCallback([](ZX::WindowEvent event, void* data) {
        switch (event) {
            case ZX::WindowEvent::Resize:
                PRINT_DEBUG("Window resized event received\n");
                break;
            case ZX::WindowEvent::Close:
                PRINT_DEBUG("Window close event received\n");
                break;
            default:
                break;
        }
    });
    
    // Initialize Vulkan - using the legacy window struct through our compatibility layer
    Vulkan vk = {};
    Window* compatWindow = *window;
    VulkanInit(&vk, compatWindow);
    
    // Main game loop
    while (window->IsRunning())
    {
        // Process window events
        window->Update();
        
        // Check if window was resized
        if (window->CheckResized())
        {
            // If the window was minimized, wait until it's restored
            if (window->GetWidth() > 0 && window->GetHeight() > 0)
            {
                PRINT("Window resized to %d x %d\n", window->GetWidth(), window->GetHeight());
                
                // Recreate swapchain with same presentation mode as before
                VulkanRecreateSwapchain(&vk, cfg.vsync ? VULKAN_PRESENT_MODE_FIFO : VULKAN_PRESENT_MODE_MAILBOX);
            }
        }
        
        // Main game update and render code would go here...
    }
    
    // Wait for the device to finish operations before cleanup
    vkDeviceWaitIdle(vk.device);
    
    // Clean up resources
    VulkanDestroy(&vk);
    
    return 0;
}