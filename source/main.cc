// Unity build includes
#include "xmath.cc"
#include "system.cc"
#include "engine.cc"

Settings settings = {
    .width = 1280,
    .height = 720,
    .window_name = "ENGINE",
};

// Entry point
int main()
{
    Engine engine;
    if (!engine.Init(settings)) {
        std::cerr << "Engine initialization failed" << std::endl;
        return -1;
    }

    return 0;
}