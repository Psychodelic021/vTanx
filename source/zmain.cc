
// Unity build includes
#include <engine.cc>
#include <system.cc>
#include <graphix.cc>
#include <xmath.cc>

EngineSettings engset = {
    // Window settings
    .sysset = {
        .width = 1280,
        .height = 720,
        .name = "Engine",
        .fullscreen = false,
        .vsync = true,
    },
};

// Entry point
int main()
{
    std::cout << "\nApplication: Start Up" << std::endl;

    Engine engine;
    engine.Init(engset);

    std::cout << "Application: Shut Down" << std::endl;

    return 0;
}