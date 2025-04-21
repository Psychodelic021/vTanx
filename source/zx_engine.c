// Single Translation Unity Build includes
#include "system.c"
#include "window.c"
#include "vulkan.c"
#include "vmath.c"

Config cfg = {
    // Window settings
    .width = 1280,
    .height = 720,
    .name = "ZXEngine",
    .fullscreen = false,
    .vsync = true,
};

// Entry point
int main()
{
    Window wnd = WindowInit(&cfg);
    Vulkan vk = {0};
    VulkanInit(&vk, &wnd);
    
    // Main game loop
    while (wnd.isRunning)
    {
        // Process window events
        WindowUpdate();
        
        // Check if window was resized
        if (WindowCheckResized(&wnd))
        {
            // If the window was minimized, wait until it's restored
            if (wnd.width > 0 && wnd.height > 0)
            {
                PRINT("Window resized to %d x %d\n", wnd.width, wnd.height);
                
                // Recreate swapchain with same presentation mode as before
                VulkanRecreateSwapchain(&vk, cfg.vsync ? VULKAN_PRESENT_MODE_FIFO : VULKAN_PRESENT_MODE_MAILBOX);
            }
        }
        
        // Main game update and render code would go here...
        
        // Exit the program if the window is closed
        if (!wnd.isRunning) break;
    }
    
    // Wait for the device to finish operations before cleanup
    vkDeviceWaitIdle(vk.device);
    
    // Clean up resources
    VulkanDestroy(&vk);
        
    return 0;
}