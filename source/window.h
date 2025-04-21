#pragma once

#include "system.h"
#include "config.h"
#include <string.h>

// Forward declaration of the global WindowProc function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

// Window state flags using enum for type safety
enum WindowStateFlags {
    WINDOW_STATE_NORMAL       = 0,
    WINDOW_STATE_MINIMIZED    = 1,
    WINDOW_STATE_MAXIMIZED    = 2,
    WINDOW_STATE_FULLSCREEN   = 4,
    WINDOW_STATE_FOCUSED      = 8,
    WINDOW_STATE_RESIZING     = 16
};

// Monitor info structure
struct MonitorInfo {
    HMONITOR handle;
    RECT     workArea;
    RECT     monitorRect;
    char     name[32];
    bool     isPrimary;
    float    dpiScale;
};

// Window event types
enum WindowEventType {
    WINDOW_EVENT_CLOSE,
    WINDOW_EVENT_RESIZE,
    WINDOW_EVENT_FOCUS,
    WINDOW_EVENT_LOST_FOCUS,
    WINDOW_EVENT_MINIMIZE,
    WINDOW_EVENT_MAXIMIZE,
    WINDOW_EVENT_RESTORE
};

// Window event callback type (C-style function pointer)
typedef void (*WindowEventCallback)(WindowEventType event, void* userData);

// Modern window class (with C++ conveniences but minimal STL)
class ModernWindow {
public:
    // Constructor and destructor
    ModernWindow();
    ~ModernWindow();
    
    // Prevent copying to avoid handle duplication issues
    ModernWindow(const ModernWindow&) = delete;
    ModernWindow& operator=(const ModernWindow&) = delete;
    
    // Allow move semantics
    ModernWindow(ModernWindow&& other);
    ModernWindow& operator=(ModernWindow&& other);

    // Static create function instead of unique_ptr factory
    static ModernWindow* Create(const Config& cfg);
    
    // Window operations
    void Update();
    void Destroy();
    bool CheckResized();
    
    // Window state accessors
    bool IsRunning() const { return m_isRunning; }
    bool IsFullscreen() const { return HasState(WINDOW_STATE_FULLSCREEN); }
    bool IsMinimized() const { return HasState(WINDOW_STATE_MINIMIZED); }
    bool IsMaximized() const { return HasState(WINDOW_STATE_MAXIMIZED); }
    bool IsFocused() const { return HasState(WINDOW_STATE_FOCUSED); }
    
    // Window property accessors
    unsigned GetWidth() const { return m_width; }
    unsigned GetHeight() const { return m_height; }
    HWND GetHandle() const { return m_handle; }
    bool GetVSync() const { return m_vsync; }
    
    // Window property setters
    void SetTitle(const char* title);
    void SetSize(unsigned width, unsigned height);
    void SetPosition(int x, int y);
    
    // Window state changes
    void Minimize();
    void Maximize();
    void Restore();
    void ToggleFullscreen();
    
    // Event handling
    void SetEventCallback(WindowEventCallback callback, void* userData) { 
        m_eventCallback = callback; 
        m_userData = userData;
    }
    
    // Convert to the legacy Window struct for compatibility
    operator Window*();
    
    // Friend function for WindowProc callback access
    friend LRESULT CALLBACK ::WindowProc(HWND, UINT, WPARAM, LPARAM);

private:
    // Internal functions
    static void SetDpiAwareness();
    bool HasState(WindowStateFlags state) const { 
        return (m_stateFlags & state) != 0;
    }
    void SetState(WindowStateFlags state) { 
        m_stateFlags |= state;
    }
    void ClearState(WindowStateFlags state) { 
        m_stateFlags &= ~state;
    }
    
    // Win32 specific
    HINSTANCE m_instance;
    HWND      m_handle;
    LPCSTR    m_className;
    LPCSTR    m_title;
    DWORD     m_style;
    DWORD     m_exStyle;
    HICON     m_icon;
    HCURSOR   m_cursor;
    
    // Window properties
    int      m_posX, m_posY;
    unsigned m_width, m_height;
    int      m_minWidth, m_minHeight;
    int      m_maxWidth, m_maxHeight;
    
    // State tracking
    bool             m_isRunning;
    bool             m_vsync;
    bool             m_resized;
    WindowStateFlags m_stateFlags;
    
    // Display/monitor info
    RECT     m_windowRect;
    RECT     m_clientRect;
    int      m_dpi;
    float    m_dpiScale;
    
    // Current monitor
    MonitorInfo m_currentMonitor;
    
    // Fullscreen state backup
    DWORD    m_savedStyle;
    RECT     m_savedRect;
    DEVMODE  m_fullscreenMode;
    
    // Event callback (C-style)
    WindowEventCallback m_eventCallback;
    void* m_userData;
    
    // Legacy Window struct for compatibility
    Window* m_legacyWindow;
};

// Legacy C-style window struct for backward compatibility
// This allows the existing Vulkan code to work while we transition
struct Window {
    // Win32 specific
    HINSTANCE instance;
    HWND      handle;
    LPCSTR    className;
    LPCSTR    title;
    DWORD     style;
    DWORD     exStyle;
    HICON     icon;
    HCURSOR   cursor;
    
    // Window properties
    int      posX, posY;
    unsigned width, height;
    int      minWidth, minHeight;
    int      maxWidth, maxHeight;
    
    // State tracking
    bool     isRunning;
    bool     vsync;
    bool     resized;
    unsigned stateFlags;
    
    // Display/monitor info
    RECT     windowRect;
    RECT     clientRect;
    int      dpi;
    float    dpiScale;
    
    // Current monitor
    MonitorInfo currentMonitor;
    
    // Fullscreen state backup
    DWORD    savedStyle;
    RECT     savedRect;
    DEVMODE  fullscreenMode;
};

// Legacy C-style window functions for backward compatibility
Window WindowInit(Config* cfg);
void WindowDestroy(Window* wnd);
void WindowUpdate();
bool WindowCheckResized(Window* wnd);

