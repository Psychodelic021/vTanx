
// Unity build includes
#include "system.cc"
#include "vmathx.cc"

// Entry point
int main() {

    printf("\nApplication started...\n");
    
    System::Window window(1600, 1000, "vTanx");

    // DATA INITIALIZE
    vec3 vertices[] = {
        {.5f, -.5f, 0.f},
        {0.f, .5f, 0.f},
        {-.5f, -.5f, 0.f}
    };

    // MAIN LOOP
    while (window.is_running) {
        window.update();


        System::SkipTime(1);
    }

    return 0;
}