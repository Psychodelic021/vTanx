#pragma once

struct Settings {
    int width;
    int height;
    const char* window_name;
};

class Engine {
public:

    System::Window window;

    Engine() = default;
    Engine(const Engine&) = delete;
    Engine operator= (const Engine&) = delete;
    ~Engine();

    bool Init(Settings);
    void Update();
    void Shutdown();
};