#include "window.h"
#include "debug.h"

namespace ZX {

// Static class name for Win32 window registration
static constexpr const char* CLASS_NAME = "ENGINE_WindowClass";

// Window default constructor
Window::Window() 
    : m_instance(nullptr)
    , m_handle(nullptr)
    , m_className(nullptr)
    , m_title(nullptr)
    , m_style(0)
    , m_exStyle(0)
    , m_icon(nullptr)
    , m_cursor(nullptr)
    , m_posX(0)
    , m_posY(0)
    , m_width(0)
    , m_height(0)
    , m_minWidth(0)
    , m_minHeight(0)
    , m_maxWidth(0)
    , m_maxHeight(0)
    , m_isRunning(false)
    , m_vsync(false)
    , m_resized(false)
    , m_stateFlags(WindowState::Normal)
    , m_windowRect({0})
    , m_clientRect({0})
    , m_dpi(0)
    , m_dpiScale(1.0f)
    , m_currentMonitor({0})
    , m_savedStyle(0)
    , m_savedRect({0})
    , m_fullscreenMode({0})
    , m_legacyWindow(nullptr)
{
    // Create a legacy window struct for compatibility with old code
    m_legacyWindow = new ::Window();
}

// Window destructor - automatically destroy window if not already done
Window::~Window() {
    if (m_handle != nullptr) {
        Destroy();
    }
    
    // Free the legacy window struct
    if (m_legacyWindow) {
        delete m_legacyWindow;
        m_legacyWindow = nullptr;
    }
}

// Move constructor
Window::Window(Window&& other) noexcept
    : m_instance(other.m_instance)
    , m_handle(other.m_handle)
    , m_className(other.m_className)
    , m_title(other.m_title)
    , m_style(other.m_style)
    , m_exStyle(other.m_exStyle)
    , m_icon(other.m_icon)
    , m_cursor(other.m_cursor)
    , m_posX(other.m_posX)
    , m_posY(other.m_posY)
    , m_width(other.m_width)
    , m_height(other.m_height)
    , m_minWidth(other.m_minWidth)
    , m_minHeight(other.m_minHeight)
    , m_maxWidth(other.m_maxWidth)
    , m_maxHeight(other.m_maxHeight)
    , m_isRunning(other.m_isRunning)
    , m_vsync(other.m_vsync)
    , m_resized(other.m_resized)
    , m_stateFlags(other.m_stateFlags)
    , m_windowRect(other.m_windowRect)
    , m_clientRect(other.m_clientRect)
    , m_dpi(other.m_dpi)
    , m_dpiScale(other.m_dpiScale)
    , m_currentMonitor(other.m_currentMonitor)
    , m_savedStyle(other.m_savedStyle)
    , m_savedRect(other.m_savedRect)
    , m_fullscreenMode(other.m_fullscreenMode)
    , m_eventCallback(std::move(other.m_eventCallback))
    , m_legacyWindow(other.m_legacyWindow)
{
    // Update the window user data to point to the new Window object
    if (m_handle) {
        SetWindowLongPtr(m_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    }
    
    // Take ownership of the legacy window
    other.m_legacyWindow = nullptr;
    
    // Clear the other window's handle to prevent it from destroying the window
    other.m_handle = nullptr;
    other.m_isRunning = false;
}

// Move assignment operator
Window& Window::operator=(Window&& other) noexcept {
    // Destroy current window if it exists
    if (m_handle != nullptr) {
        Destroy();
    }
    
    // Free the legacy window struct
    if (m_legacyWindow) {
        delete m_legacyWindow;
    }
    
    // Copy all properties
    m_instance = other.m_instance;
    m_handle = other.m_handle;
    m_className = other.m_className;
    m_title = other.m_title;
    m_style = other.m_style;
    m_exStyle = other.m_exStyle;
    m_icon = other.m_icon;
    m_cursor = other.m_cursor;
    m_posX = other.m_posX;
    m_posY = other.m_posY;
    m_width = other.m_width;
    m_height = other.m_height;
    m_minWidth = other.m_minWidth;
    m_minHeight = other.m_minHeight;
    m_maxWidth = other.m_maxWidth;
    m_maxHeight = other.m_maxHeight;
    m_isRunning = other.m_isRunning;
    m_vsync = other.m_vsync;
    m_resized = other.m_resized;
    m_stateFlags = other.m_stateFlags;
    m_windowRect = other.m_windowRect;
    m_clientRect = other.m_clientRect;
    m_dpi = other.m_dpi;
    m_dpiScale = other.m_dpiScale;
    m_currentMonitor = other.m_currentMonitor;
    m_savedStyle = other.m_savedStyle;
    m_savedRect = other.m_savedRect;
    m_fullscreenMode = other.m_fullscreenMode;
    m_eventCallback = std::move(other.m_eventCallback);
    m_legacyWindow = other.m_legacyWindow;
    
    // Update the window user data to point to the new Window object
    if (m_handle) {
        SetWindowLongPtr(m_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    }
    
    // Take ownership of the legacy window
    other.m_legacyWindow = nullptr;
    
    // Clear the other window's handle to prevent it from destroying the window
    other.m_handle = nullptr;
    other.m_isRunning = false;
    
    return *this;
}

// Factory method to create a window
std::unique_ptr<Window> Window::Create(const Config& cfg) {
    PRINT_DEBUG("Window: initializing...\n");
    
    auto window = std::make_unique<Window>();
    
    // Set window properties
    window->m_title = cfg.name;
    window->m_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    window->m_instance = GetModuleHandle(0);
    window->m_className = CLASS_NAME;
    window->m_vsync = cfg.vsync;
    window->m_isRunning = true;

    WNDCLASS wc = {
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = ::WindowProc,
        .hInstance = window->m_instance,
        .hCursor = LoadCursor(nullptr, IDC_ARROW),
        .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
        .lpszClassName = CLASS_NAME,
    };
    
    if (!RegisterClass(&wc)) {
        PRINT_ERROR("Window error: registration failed\n");
        __debugbreak();
        return nullptr;
    }

    // Set DPI awareness context for the window
    SetDpiAwareness();

    int dspWidth = GetSystemMetrics(SM_CXSCREEN);
    int dspHeight = GetSystemMetrics(SM_CYSCREEN);

    if (cfg.fullscreen) {
        // Set the window style to fullscreen
        window->m_style = WS_POPUP | WS_VISIBLE;
        window->m_posX = 0;
        window->m_posY = 0;

        // Set the window size to the display size
        window->m_windowRect = {0, 0, dspWidth, dspHeight};
        AdjustWindowRect(&window->m_windowRect, window->m_style, 0);

        window->m_width = dspWidth;
        window->m_height = dspHeight;

        // Set the display mode for fullscreen
        window->m_fullscreenMode = DEVMODE{
            .dmSize = sizeof(DEVMODE),
            .dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT,
            .dmBitsPerPel = 32,
            .dmPelsWidth = static_cast<DWORD>(window->m_width),
            .dmPelsHeight = static_cast<DWORD>(window->m_height)
        };

        ChangeDisplaySettings(&window->m_fullscreenMode, CDS_FULLSCREEN);
        window->SetState(WindowState::Fullscreen);
    } else {
        window->m_width = cfg.width;
        window->m_height = cfg.height;

        window->m_posX = (dspWidth - window->m_width) / 2;
        window->m_posY = (dspHeight - window->m_height) / 2;

        window->m_windowRect = {0, 0, static_cast<LONG>(window->m_width), static_cast<LONG>(window->m_height)};
        AdjustWindowRect(&window->m_windowRect, window->m_style, 0);
    }

    window->m_handle = CreateWindow(
        CLASS_NAME, 
        window->m_title, 
        window->m_style, 
        window->m_posX, 
        window->m_posY,
        window->m_windowRect.right - window->m_windowRect.left, 
        window->m_windowRect.bottom - window->m_windowRect.top,
        nullptr, 
        nullptr, 
        window->m_instance, 
        nullptr
    );

    if (!window->m_handle) {
        PRINT_ERROR("Window error: Window creation failed");
        __debugbreak();
        return nullptr;
    }

    // Store pointer to the window object in window user data
    SetWindowLongPtr(window->m_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window.get()));

    ShowWindow(window->m_handle, SW_SHOW);
    SetFocus(window->m_handle);
    UpdateWindow(window->m_handle);
    
    // Set initial states
    window->SetState(WindowState::Focused);

    // Setup the legacy window struct for compatibility
    window->m_legacyWindow->handle = window->m_handle;
    window->m_legacyWindow->instance = window->m_instance;
    window->m_legacyWindow->className = window->m_className;
    window->m_legacyWindow->title = window->m_title;
    window->m_legacyWindow->style = window->m_style;
    window->m_legacyWindow->width = window->m_width;
    window->m_legacyWindow->height = window->m_height;
    window->m_legacyWindow->posX = window->m_posX;
    window->m_legacyWindow->posY = window->m_posY;
    window->m_legacyWindow->isRunning = window->m_isRunning;
    window->m_legacyWindow->vsync = window->m_vsync;
    window->m_legacyWindow->resized = window->m_resized;
    
    PRINT_INFO("Window: ON\n");

    return window;
}

void Window::Destroy() {
    if (m_handle != nullptr) {
        m_isRunning = false;

        // If fullscreen, restore display settings
        if (HasState(WindowState::Fullscreen)) {
            ChangeDisplaySettings(nullptr, 0);
        }

        // Destroy the window and set the handle to NULL
        DestroyWindow(m_handle);
        m_handle = nullptr;

        // Unregister the window class
        UnregisterClass(m_className, m_instance);
        
        // Update the legacy window
        if (m_legacyWindow) {
            m_legacyWindow->handle = nullptr;
            m_legacyWindow->isRunning = false;
        }
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
    if (m_resized) {
        m_resized = false;
        if (m_legacyWindow) {
            m_legacyWindow->resized = false;
        }
        return true;
    }
    return false;
}

// Set window title
void Window::SetTitle(const char* title) {
    if (m_handle && title) {
        m_title = title;
        if (m_legacyWindow) {
            m_legacyWindow->title = title;
        }
        SetWindowText(m_handle, m_title);
    }
}

// Set window size
void Window::SetSize(unsigned width, unsigned height) {
    if (m_handle && width > 0 && height > 0) {
        m_width = width;
        m_height = height;
        
        if (m_legacyWindow) {
            m_legacyWindow->width = width;
            m_legacyWindow->height = height;
        }
        
        RECT rect = {0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
        AdjustWindowRect(&rect, m_style, FALSE);
        
        SetWindowPos(
            m_handle, 
            nullptr, 
            0, 0, 
            rect.right - rect.left, 
            rect.bottom - rect.top, 
            SWP_NOMOVE | SWP_NOZORDER
        );
    }
}

// Set window position
void Window::SetPosition(int x, int y) {
    if (m_handle) {
        m_posX = x;
        m_posY = y;
        
        if (m_legacyWindow) {
            m_legacyWindow->posX = x;
            m_legacyWindow->posY = y;
        }
        
        SetWindowPos(
            m_handle,
            nullptr,
            x, y,
            0, 0,
            SWP_NOSIZE | SWP_NOZORDER
        );
    }
}

// Minimize the window
void Window::Minimize() {
    if (m_handle) {
        ShowWindow(m_handle, SW_MINIMIZE);
    }
}

// Maximize the window
void Window::Maximize() {
    if (m_handle) {
        ShowWindow(m_handle, SW_MAXIMIZE);
    }
}

// Restore the window
void Window::Restore() {
    if (m_handle) {
        ShowWindow(m_handle, SW_RESTORE);
    }
}

// Toggle fullscreen mode
void Window::ToggleFullscreen() {
    if (!m_handle) return;
    
    if (HasState(WindowState::Fullscreen)) {
        // Return to windowed mode
        ChangeDisplaySettings(nullptr, 0);
        
        // Restore window style
        SetWindowLong(m_handle, GWL_STYLE, m_savedStyle);
        
        // Restore window position and size
        SetWindowPos(
            m_handle, 
            HWND_TOP, 
            m_savedRect.left, 
            m_savedRect.top, 
            m_savedRect.right - m_savedRect.left, 
            m_savedRect.bottom - m_savedRect.top, 
            SWP_FRAMECHANGED
        );
        
        ClearState(WindowState::Fullscreen);
    }
    else {
        // Save current window state
        m_savedStyle = GetWindowLong(m_handle, GWL_STYLE);
        GetWindowRect(m_handle, &m_savedRect);
        
        // Switch to fullscreen mode
        MONITORINFO mi = { sizeof(MONITORINFO) };
        GetMonitorInfo(MonitorFromWindow(m_handle, MONITOR_DEFAULTTONEAREST), &mi);
        
        SetWindowLong(m_handle, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        SetWindowPos(
            m_handle, 
            HWND_TOP, 
            mi.rcMonitor.left, 
            mi.rcMonitor.top, 
            mi.rcMonitor.right - mi.rcMonitor.left, 
            mi.rcMonitor.bottom - mi.rcMonitor.top, 
            SWP_FRAMECHANGED
        );
        
        SetState(WindowState::Fullscreen);
    }
}

// Set DPI awareness for high DPI displays
void Window::SetDpiAwareness() {
    // Enable Per-Monitor DPI awareness
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
}

// Cast operator to convert ZX::Window to legacy Window*
Window::operator ::Window*() {
    if (m_legacyWindow) {
        // Always ensure the legacy struct has the latest data
        m_legacyWindow->handle = m_handle;
        m_legacyWindow->instance = m_instance;
        m_legacyWindow->width = m_width;
        m_legacyWindow->height = m_height;
        m_legacyWindow->isRunning = m_isRunning;
        m_legacyWindow->vsync = m_vsync;
        m_legacyWindow->resized = m_resized;
    }
    return m_legacyWindow;
}

} // namespace ZX

// Global window procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    // Get the Window object pointer from the window's user data
    ZX::Window* window = reinterpret_cast<ZX::Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    
    // Handle messages that come before window creation or after destruction
    if (!window) {
        return DefWindowProc(hwnd, message, wparam, lparam);
    }

    switch (message) {
        case WM_CLOSE: {
            DestroyWindow(hwnd);
            
            // Notify via callback if registered
            if (window->m_eventCallback) {
                window->m_eventCallback(ZX::WindowEvent::Close, nullptr);
            }
            break;
        }

        case WM_DESTROY: {
            window->m_isRunning = false;
            PostQuitMessage(0);
            break;
        }
        
        case WM_SIZE: {
            // Update the window size
            window->m_width = LOWORD(lparam);
            window->m_height = HIWORD(lparam);
            
            // Update legacy window struct
            if (window->m_legacyWindow) {
                window->m_legacyWindow->width = window->m_width;
                window->m_legacyWindow->height = window->m_height;
            }
            
            // Update window state
            if (wparam == SIZE_MINIMIZED) {
                window->SetState(ZX::WindowState::Minimized);
                window->ClearState(ZX::WindowState::Maximized);
                
                // Notify via callback if registered
                if (window->m_eventCallback) {
                    window->m_eventCallback(ZX::WindowEvent::Minimize, nullptr);
                }
            }
            else if (wparam == SIZE_MAXIMIZED) {
                window->ClearState(ZX::WindowState::Minimized);
                window->SetState(ZX::WindowState::Maximized);
                
                // Notify via callback if registered
                if (window->m_eventCallback) {
                    window->m_eventCallback(ZX::WindowEvent::Maximize, nullptr);
                }
            }
            else if (wparam == SIZE_RESTORED) {
                window->ClearState(ZX::WindowState::Minimized);
                window->ClearState(ZX::WindowState::Maximized);
                
                // Notify via callback if registered
                if (window->m_eventCallback) {
                    window->m_eventCallback(ZX::WindowEvent::Restore, nullptr);
                }
            }
            
            // Set a flag to indicate the window has been resized
            if (window->m_width > 0 && window->m_height > 0) {
                window->m_resized = true;
                
                // Update legacy window struct
                if (window->m_legacyWindow) {
                    window->m_legacyWindow->resized = true;
                }
                
                // Notify via callback if registered
                if (window->m_eventCallback) {
                    window->m_eventCallback(ZX::WindowEvent::Resize, nullptr);
                }
            }
            break;
        }
        
        case WM_SETFOCUS: {
            window->SetState(ZX::WindowState::Focused);
            
            // Notify via callback if registered
            if (window->m_eventCallback) {
                window->m_eventCallback(ZX::WindowEvent::Focus, nullptr);
            }
            break;
        }
        
        case WM_KILLFOCUS: {
            window->ClearState(ZX::WindowState::Focused);
            
            // Notify via callback if registered
            if (window->m_eventCallback) {
                window->m_eventCallback(ZX::WindowEvent::LostFocus, nullptr);
            }
            break;
        }
        
        case WM_ENTERSIZEMOVE: {
            window->SetState(ZX::WindowState::Resizing);
            break;
        }
        
        case WM_EXITSIZEMOVE: {
            window->ClearState(ZX::WindowState::Resizing);
            break;
        }

        default:
            break;
    }

    return DefWindowProc(hwnd, message, wparam, lparam);
}

// Legacy C-style window functions implementations for backward compatibility
Window WindowInit(ZX::Config* cfg) {
    // Create a C++ window wrapper
    auto window = ZX::Window::Create(*cfg);
    if (!window) {
        ::Window emptyWindow = {};
        return emptyWindow;
    }
    
    // Return the legacy window struct by using the conversion operator
    return *static_cast<::Window*>(*window);
}

void WindowDestroy(Window* wnd) {
    // Nothing to do - destruction will be handled by the C++ window class
}

void WindowUpdate() {
    // This global function now does nothing as we use the window object directly
    // Left for backward compatibility
}

bool WindowCheckResized(Window* wnd) {
    if (wnd && wnd->resized) {
        wnd->resized = false;
        return true;
    }
    return false;
}