#pragma once

class Config {
public:
    int width;
    int height;
    const char* name;
    bool fullscreen;
    bool vsync;
    
    // Constructor with default values
    Config(int w = 1280, int h = 720, const char* n = "ZXEngine", bool fs = false, bool vs = true)
        : width(w), height(h), name(n), fullscreen(fs), vsync(vs) {}
};
