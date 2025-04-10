
// Single Translation Unity Build includes
#include "system.c"
#include "gfx_vulkan.c"
#include "vmath.c"

Settings settings = {
    // Window settings
    .width = 1280,
    .height = 720,
    .name = "ZXEngine",
    .fullscreen = false,
    .vsync = true,
};

// Entry pointW
int main()
{
    PRINT_DEBUG("\nEngine: Start Up\n");

    Window wnd = WindowInit(&settings);
    Vulkan gfx = VulkanInit(&wnd);

    PRINT_INFO("Engine: Running...\n");

    VulkanDestroy(&gfx);
    WindowDestroy(&wnd);
        
    return 0;
}