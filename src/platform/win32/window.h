#pragma once

#include "system.h"
#include "config.h"
#include <string>

// Window state flags
enum WindowState {
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

class Window {
public:
    // Constructor and destructor
    Window() = default;
    ~Window();
    
    // Initialization
    static Window Create(const Config& cfg);
    void Destroy();
    
    // Window operations
    void Update();
    bool CheckResized();
    
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

    // Friend function for WindowProc callback access
    friend LRESULT CALLBACK WindowProc(HWND, unsigned, WPARAM, LPARAM);

private:
    // Internal functions
    static void SetDpiAwareness();
};

// Global window procedure function
LRESULT CALLBACK WindowProc(HWND hwnd, unsigned message, WPARAM wparam, LPARAM lparam);

