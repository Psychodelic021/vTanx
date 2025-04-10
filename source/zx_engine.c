
// Single Translation Unity Build includes
#include <system.c>
#include <vulkangfx.c>
#include <vmath.c>

Settings settings = {
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
    PRINT_DEBUG("\nEngine: Start Up\n");

    Window wnd = WindowInit(&settings);
    Vulkan gfx = VulkanInit(&system, &settings);

    PRINT_INFO("Engine: Running...\n");
        
    return 0;
} 