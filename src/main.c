// API includes

// Local includes
#include "win32.c"

// Entry point
int main() {
    
    WindowState window;
    Win32_WindowInit(&window, 800, 600, "vTanx");
    
    return 0;
}