
// Single Translation Unity Build includes
#include <system.cpp>
#include <vulkan.cpp>
#include <vmx.cpp>

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

    System system(settings);
    Vulkan gfx(&system, &settings);

    PRINT_INFO("Engine: Running...\n");
        
    return 0;
} 