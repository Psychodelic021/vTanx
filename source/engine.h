#pragma once

#include <common.h>
#include <system.h>
#include <graphix.h>

struct EngineSettings {
    SystemSettings sysset;
    GraphixSettings gfxset;
};

class Engine {

    private:
        System system;
        Graphix gfx;
        
    public:
        Engine() = default;
        Engine(const Engine&) = delete;
        Engine& operator=(const Engine&) = delete;
        ~Engine();

        void Init(EngineSettings);
        void Destroy();
};