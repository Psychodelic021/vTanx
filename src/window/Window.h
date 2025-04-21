#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <functional>

// Forward declaration
class Input;

// Window class for managing Win32 windows
class Window {
public:
    // Nested struct for window configuration
    struct WindowConfig {
        std::wstring title;
        int width;
        int height;
        bool fullscreen;
        bool vsync;
        bool resizable;
        
        // Constructor with default values
        WindowConfig();
    };

    // Constructor - creates a window with the specified configuration
    Window(const WindowConfig& config = WindowConfig());
    
    // Destructor - cleans up window resources
    ~Window();
    
    // Show the window and begin processing messages
    void Show();
    
    // Process window messages
    bool ProcessMessages();
    
    // Toggle fullscreen mode
    void SetFullscreen(bool fullscreen);
    
    // Resize the window
    void Resize(int width, int height);
    
    // Set window title
    void SetTitle(const std::wstring& title);
    
    // Handle window messages
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    // Getters
    HWND GetHandle() const;
    int GetWidth() const;
    int GetHeight() const;
    bool IsFullscreen() const;
    bool IsVSync() const;
    
    // Set the input system to receive events
    void SetInputSystem(Input* input);
    
    // Callback setters
    using ResizeCallback = std::function<void(int, int)>;
    using KeyCallback = std::function<void(int, bool)>;
    using MouseMoveCallback = std::function<void(int, int)>;
    using MouseButtonCallback = std::function<void(int, bool)>;
    using MouseWheelCallback = std::function<void(float)>;
    
    void SetResizeCallback(ResizeCallback callback);
    void SetKeyCallback(KeyCallback callback);
    void SetMouseMoveCallback(MouseMoveCallback callback);
    void SetMouseButtonCallback(MouseButtonCallback callback);
    void SetMouseWheelCallback(MouseWheelCallback callback);
    
private:
    HWND m_hwnd = nullptr;
    WindowConfig m_config;
    RECT m_preFullscreenRect = {};
    Input* m_input = nullptr;
    
    // Callbacks
    ResizeCallback m_resizeCallback;
    KeyCallback m_keyCallback;
    MouseMoveCallback m_mouseMoveCallback;
    MouseButtonCallback m_mouseButtonCallback;
    MouseWheelCallback m_mouseWheelCallback;
    
    // Mouse tracking
    bool m_mouseTracked = false;
};

// Global window procedure to handle Windows messages
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);