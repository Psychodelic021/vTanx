#pragma once

#include <engine.h>

void Engine::Init(EngineSettings engset)
{
    std::cout << "Engine: Initializing..." << std::endl;

    system.Init(engset.sysset);
    gfx.Init(system.GetWindowHandle());

    std::cout << "Engine: OK" << std::endl;
}

void Engine::Destroy()
{
    std::cout << "Engine: Shutting down..." << std::endl;
}

Engine::~Engine()
{
    Destroy();
}

