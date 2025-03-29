#pragma once

#include <iostream>

#include "system.h"
#include "engine.h"

bool Engine::Init(Settings set)
{
    std::cout << "\nEngine: initializing..." << std::endl;

    if (!window.Init(set.width, set.height, set.window_name)) {
        std::cerr << "Error: Window initialization failed\n";
        return false;
    }

    return true;
}

void Engine::Update()
{

}

void Engine::Shutdown()
{
    
    

    std::cout << "\nEngine: shutting down..." << std::endl;
}

Engine::~Engine()
{
    Shutdown();
}

