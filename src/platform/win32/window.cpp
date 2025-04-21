#include "window.h"
#include "debug.h"

// Static class name for Win32 window registration
static constexpr const char* CLASS_NAME = "ENGINE_WindowClass";

// Window destructor
Window::~Window() {
    if (isRunning) {
        Destroy();
    }
}

// Window initialization
Window Window::Create(const Config& cfg) {
    PRINT_DEBUG("Window: initializing...\n");
    
    Window wnd;

    // Set window properties
    wnd.title = cfg.name;
    wnd.style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    wnd.instance = GetModuleHandle(0);
    wnd.className = CLASS_NAME;
    wnd.vsync = cfg.vsync;
    wnd.isRunning = true;

    WNDCLASS wc = {
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = WindowProc,
        .hInstance = wnd.instance,
        .hCursor = LoadCursor(0, IDC_ARROW),
        .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
        .lpszClassName = CLASS_NAME,
    };
    
    if (!RegisterClass(&wc)) {
        PRINT_ERROR("Window error: registration failed\n");
        __debugbreak();
    }

    // Set DPI awareness context for the window
    SetDpiAwareness();

    int dspWidth = GetSystemMetrics(SM_CXSCREEN);
    int dspHeight = GetSystemMetrics(SM_CYSCREEN);

    if (cfg.fullscreen) {
        // Set the window style to fullscreen
        wnd.style = WS_POPUP | WS_VISIBLE;
        wnd.posX = 0;
        wnd.posY = 0;

        // Set the window size to the display size
        wnd.windowRect = {0, 0, dspWidth, dspHeight};
        AdjustWindowRect(&wnd.windowRect, wnd.style, 0);

        wnd.width = dspWidth;
        wnd.height = dspHeight;

        // Set the display mode for fullscreen
        wnd.fullscreenMode = DEVMODE{
            .dmSize = sizeof(DEVMODE),
            .dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT,
            .dmBitsPerPel = 32,
            .dmPelsWidth = static_cast<DWORD>(wnd.width),
            .dmPelsHeight = static_cast<DWORD>(wnd.height)
        };

        ChangeDisplaySettings(&wnd.fullscreenMode, CDS_FULLSCREEN);
    } else {
        wnd.width = cfg.width;
        wnd.height = cfg.height;

        wnd.posX = (dspWidth - wnd.width) / 2;
        wnd.posY = (dspHeight - wnd.height) / 2;

        wnd.windowRect = {0, 0, static_cast<LONG>(wnd.width), static_cast<LONG>(wnd.height)};
        AdjustWindowRect(&wnd.windowRect, wnd.style, 0);
    }

    wnd.handle = CreateWindow(
        CLASS_NAME, 
        wnd.title, 
        wnd.style, 
        wnd.posX, 
        wnd.posY,
        wnd.windowRect.right - wnd.windowRect.left, 
        wnd.windowRect.bottom - wnd.windowRect.top,
        nullptr, 
        nullptr, 
        wnd.instance, 
        nullptr
    );

    if (!wnd.handle) {
        PRINT_ERROR("Window error: Window creation failed");
        __debugbreak();
    }

    // Store pointer to the window object in window user data
    SetWindowLongPtr(wnd.handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&wnd));

    ShowWindow(wnd.handle, SW_SHOW);
    SetFocus(wnd.handle);
    UpdateWindow(wnd.handle);

    PRINT_INFO("Window: ON\n");

    return wnd;
}

void Window::Destroy() {
    if (handle != nullptr) {
        isRunning = false;

        // If fullscreen, restore display settings
        if (fullscreenMode.dmSize > 0) {
            ChangeDisplaySettings(nullptr, 0);
        }

        // Destroy the window and set the handle to NULL
        DestroyWindow(handle);
        handle = nullptr;

        // Unregister the window class
        UnregisterClass(className, instance);
    }
    
    PRINT_TRACE("Window: OFF\n");
}

// Window message pump
void Window::Update() {
    MSG event = {};
    while (PeekMessage(&event, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&event);
        DispatchMessage(&event);
    }
}

// Check if window was resized and reset the flag
bool Window::CheckResized() {
    if (resized) {
        resized = false;
        return true;
    }
    return false;
}

// Set DPI awareness for high DPI displays
void Window::SetDpiAwareness() {
    // Enable Per-Monitor DPI awareness
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}

// Window procedure implementation
LRESULT CALLBACK WindowProc(HWND hwnd, unsigned message, WPARAM wparam, LPARAM lparam) {
    // Get the Window object pointer from the window's user data
    Window* wnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (message) {
        case WM_CLOSE: {
            DestroyWindow(hwnd);
            break;
        }

        case WM_DESTROY: {
            if (wnd) {
                wnd->isRunning = false;
            }
            PostQuitMessage(0);
            break;
        }
        
        case WM_SIZE: {
            if (wnd && wparam != SIZE_MINIMIZED) {
                // Update the window size
                wnd->width = LOWORD(lparam);
                wnd->height = HIWORD(lparam);
                
                // Set a flag to indicate the window has been resized
                if (wnd->width > 0 && wnd->height > 0) {
                    wnd->resized = true;
                }
            }
            break;
        }

        default:
            break;
    }

    return DefWindowProc(hwnd, message, wparam, lparam);
}