#include "Window.h"
#include <stdexcept>
#include <windowsx.h> // For GET_X_LPARAM, GET_Y_LPARAM, etc.
#include "../input/Input.h"

// WindowConfig constructor implementation
Window::WindowConfig::WindowConfig() : 
    title(L"Game Engine Window"),
    width(800),
    height(600),
    fullscreen(false),
    vsync(true),
    resizable(true)
{
}

// Window constructor implementation
Window::Window(const WindowConfig& config) : m_config(config) {
    // Register window class
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.hCursor = LoadCursorW(nullptr, (LPCWSTR)IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = L"GameEngineWindowClass";
    wc.hIcon = LoadIconW(nullptr, (LPCWSTR)IDI_APPLICATION);
    
    if (!RegisterClassExW(&wc)) {
        throw std::runtime_error("Failed to register window class");
    }
    
    // Setup window style
    DWORD style = WS_OVERLAPPEDWINDOW;
    if (!m_config.resizable) {
        style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
    }
    
    // Calculate window size based on desired client area
    RECT rect = { 0, 0, static_cast<LONG>(m_config.width), static_cast<LONG>(m_config.height) };
    AdjustWindowRect(&rect, style, FALSE);
    
    // Create window
    m_hwnd = CreateWindowExW(
        0,                              // Extended style
        wc.lpszClassName,               // Window class name
        m_config.title.c_str(),         // Window title
        style,                          // Window style
        CW_USEDEFAULT,                  // X position
        CW_USEDEFAULT,                  // Y position
        rect.right - rect.left,         // Width
        rect.bottom - rect.top,         // Height
        nullptr,                        // Parent window
        nullptr,                        // Menu
        wc.hInstance,                   // Instance handle
        this                            // Additional data
    );
    
    if (!m_hwnd) {
        throw std::runtime_error("Failed to create window");
    }
    
    // Store a pointer to the window object in the window's user data
    SetWindowLongPtrW(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    
    // Handle fullscreen if needed
    if (m_config.fullscreen) {
        SetFullscreen(true);
    }
}

// Window destructor implementation
Window::~Window() {
    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
}

// Show window implementation
void Window::Show() {
    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
}

// Process messages implementation
bool Window::ProcessMessages() {
    MSG msg = {};
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false; // Signal to stop the application
        }
        
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return true;
}

// Set fullscreen implementation
void Window::SetFullscreen(bool fullscreen) {
    if (fullscreen == m_config.fullscreen) return;
    
    m_config.fullscreen = fullscreen;
    
    if (fullscreen) {
        // Store current window position and size
        GetWindowRect(m_hwnd, &m_preFullscreenRect);
        
        // Get the monitor info
        HMONITOR monitor = MonitorFromWindow(m_hwnd, MONITOR_DEFAULTTONEAREST);
        MONITORINFO monitorInfo = {};
        monitorInfo.cbSize = sizeof(monitorInfo);
        GetMonitorInfoW(monitor, &monitorInfo);
        
        // Set full screen style
        SetWindowLongW(m_hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
        
        // Set window to cover the entire monitor
        SetWindowPos(
            m_hwnd,
            HWND_TOP,
            monitorInfo.rcMonitor.left,
            monitorInfo.rcMonitor.top,
            monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left,
            monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top,
            SWP_FRAMECHANGED
        );
    } else {
        // Restore windowed mode
        SetWindowLongW(m_hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
        
        // Restore the window position and size
        SetWindowPos(
            m_hwnd,
            nullptr,
            m_preFullscreenRect.left,
            m_preFullscreenRect.top,
            m_preFullscreenRect.right - m_preFullscreenRect.left,
            m_preFullscreenRect.bottom - m_preFullscreenRect.top,
            SWP_FRAMECHANGED | SWP_NOZORDER
        );
    }
}

// Resize implementation
void Window::Resize(int width, int height) {
    if (m_config.fullscreen) return; // Ignore resize in fullscreen mode
    
    m_config.width = width;
    m_config.height = height;
    
    // Calculate window size based on desired client area
    RECT rect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
    AdjustWindowRect(&rect, GetWindowLongW(m_hwnd, GWL_STYLE), FALSE);
    
    SetWindowPos(
        m_hwnd,
        nullptr,
        0, 0,
        rect.right - rect.left,
        rect.bottom - rect.top,
        SWP_NOMOVE | SWP_NOZORDER
    );
}

// Set title implementation
void Window::SetTitle(const std::wstring& title) {
    m_config.title = title;
    SetWindowTextW(m_hwnd, title.c_str());
}

// Handle message implementation
LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CLOSE:
            // Handle window close request
            DestroyWindow(m_hwnd);
            return 0;
            
        case WM_DESTROY:
            // Post quit message when window is destroyed
            PostQuitMessage(0);
            return 0;
            
        case WM_SIZE:
            // Handle window resize events
            if (wParam != SIZE_MINIMIZED) {
                int width = LOWORD(lParam);
                int height = HIWORD(lParam);
                m_config.width = width;
                m_config.height = height;
                
                // Call resize callback if set
                if (m_resizeCallback) {
                    m_resizeCallback(width, height);
                }
            }
            return 0;
            
        case WM_KEYDOWN:
            // Handle key press events
            if (m_input) {
                m_input->OnKeyEvent(static_cast<int>(wParam), true);
            }
            if (m_keyCallback) {
                m_keyCallback(static_cast<int>(wParam), true);
            }
            return 0;
            
        case WM_KEYUP:
            // Handle key release events
            if (m_input) {
                m_input->OnKeyEvent(static_cast<int>(wParam), false);
            }
            if (m_keyCallback) {
                m_keyCallback(static_cast<int>(wParam), false);
            }
            return 0;
            
        case WM_MOUSEMOVE: {
            // Start tracking mouse events if not already tracking
            if (!m_mouseTracked) {
                TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hwnd, 0 };
                TrackMouseEvent(&tme);
                m_mouseTracked = true;
            }
            
            // Get mouse coordinates
            int xPos = GET_X_LPARAM(lParam);
            int yPos = GET_Y_LPARAM(lParam);
            
            // Send to input system
            if (m_input) {
                m_input->OnMouseMoveEvent(xPos, yPos);
            }
            
            // Call mouse move callback if set
            if (m_mouseMoveCallback) {
                m_mouseMoveCallback(xPos, yPos);
            }
            return 0;
        }
            
        case WM_MOUSELEAVE:
            // Mouse left the client area
            m_mouseTracked = false;
            return 0;
            
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP: {
            bool pressed = (uMsg == WM_LBUTTONDOWN || uMsg == WM_MBUTTONDOWN || 
                           uMsg == WM_RBUTTONDOWN || uMsg == WM_XBUTTONDOWN);
                
            // Send to input system
            if (m_input) {
                m_input->OnMouseButtonEvent(uMsg, pressed);
            }
                
            // Call mouse button callback if set
            if (m_mouseButtonCallback) {
                m_mouseButtonCallback(uMsg, pressed);
            }
                
            return 0;
        }
            
        case WM_MOUSEWHEEL: {
            // Get wheel delta
            float delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA;
                
            // Send to input system
            if (m_input) {
                m_input->OnMouseScrollEvent(delta);
            }
                
            // Call mouse wheel callback if set
            if (m_mouseWheelCallback) {
                m_mouseWheelCallback(delta);
            }
                
            return 0;
        }
    }
    
    return DefWindowProcW(m_hwnd, uMsg, wParam, lParam);
}

// Getters implementation
HWND Window::GetHandle() const { 
    return m_hwnd; 
}

int Window::GetWidth() const { 
    return m_config.width; 
}

int Window::GetHeight() const { 
    return m_config.height; 
}

bool Window::IsFullscreen() const { 
    return m_config.fullscreen; 
}

bool Window::IsVSync() const { 
    return m_config.vsync; 
}

// Set input system
void Window::SetInputSystem(Input* input) {
    m_input = input;
}

// Callback setters implementation
void Window::SetResizeCallback(ResizeCallback callback) { 
    m_resizeCallback = callback; 
}

void Window::SetKeyCallback(KeyCallback callback) { 
    m_keyCallback = callback; 
}

void Window::SetMouseMoveCallback(MouseMoveCallback callback) {
    m_mouseMoveCallback = callback;
}

void Window::SetMouseButtonCallback(MouseButtonCallback callback) {
    m_mouseButtonCallback = callback;
}

void Window::SetMouseWheelCallback(MouseWheelCallback callback) {
    m_mouseWheelCallback = callback;
}

// Global window procedure implementation
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    // Get the window instance pointer from user data
    Window* window = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    
    // Special case for WM_CREATE
    if (uMsg == WM_CREATE) {
        // Extract the Window instance from create params
        CREATESTRUCTW* createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
        window = reinterpret_cast<Window*>(createStruct->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    }
    
    // If we have a valid window instance, let it handle the message
    if (window) {
        return window->HandleMessage(uMsg, wParam, lParam);
    }
    
    // Default handling
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}