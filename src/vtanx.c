
// Unity build includes
#include "win32.c"
#include "vk.c"

// Entry point
int main() {
    
    WindowState window = {0};
    Win32_WindowInit(&window, 1000, 600, "vTanx");

    VulkanContext vkcontext = {0};
    VulkanInit(&vkcontext, &window);
    
    while (window.is_running) {
        Win32_WindowUpdate(&window);
    }

    return 0;
}