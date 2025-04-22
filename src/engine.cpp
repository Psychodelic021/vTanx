// Engine.cpp - Main entry point for the engine
// This file is using Single Translation Unit (STU) approach
// All implementation files are included here

// Define NOMINMAX to prevent Windows.h from defining min/max macros
#define NOMINMAX
#define GLM_ENABLE_EXPERIMENTAL // Enable experimental GLM extensions

// Include standard headers
#include <windows.h>
#include <string>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <chrono>
#include <algorithm> // For std::min, std::max

// Include GLM headers with correct path
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtc/type_ptr.hpp"
#include "../vendor/glm/gtx/euler_angles.hpp"

// Include window implementation
#include "window/Window.h"

// Include renderer implementation
#include "renderer/VulkanRenderer.h"

// Include input system
#include "input/Input.h"

// Include camera system
#include "camera/Camera.h"

// Include mesh system
#include "mesh/Mesh.h"

// Must include implementation files for STU build
#include "window/Window.cpp"
#include "renderer/VulkanRenderer.cpp"
#include "input/Input.cpp"
#include "camera/Camera.cpp"
#include "mesh/Mesh.cpp"

// Main entry point for a Windows application
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    try {
        // Create a window with default configuration
        Window window;
        
        // Set custom window title
        window.SetTitle(L"3D Game Engine - Vulkan");

        // Create input system and connect to window
        Input input(&window);
        window.SetInputSystem(&input);
        
        // Create camera with the window's aspect ratio
        Camera camera(static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight()));
        camera.SetPosition(glm::vec3(0.0f, 1.0f, -5.0f));
        
        // Create Vulkan renderer
        VulkanRenderer renderer(&window);
        
        // Initialize Vulkan
        if (!renderer.Initialize()) {
            throw std::runtime_error("Failed to initialize Vulkan renderer");
        }
        
        // Initialize the mesh manager
        MeshManager::GetInstance().Initialize(&renderer);
        
        // Create a triangle mesh
        std::shared_ptr<Mesh> triangleMesh = MeshManager::GetInstance().CreateMesh(MeshPrimitiveType::TRIANGLE);
        if (!triangleMesh) {
            throw std::runtime_error("Failed to create triangle mesh");
        }
        
        // Set resize callback to update camera's aspect ratio
        window.SetResizeCallback([&camera](int width, int height) {
            float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
            camera.SetAspectRatio(aspectRatio);
            OutputDebugStringA(("Window resized: " + std::to_string(width) + "x" + std::to_string(height) + "\n").c_str());
        });
        
        // Set key callback
        window.SetKeyCallback([&window, &camera](int key, bool pressed) {
            // Handle key events
            if (pressed && key == VK_ESCAPE) {
                DestroyWindow(window.GetHandle());
            }
            if (pressed && key == VK_F11) {
                window.SetFullscreen(!window.IsFullscreen());
            }
            
            // Camera mode switching
            if (pressed && key == '1') {
                camera.SetMode(CameraMode::FirstPerson);
                OutputDebugStringA("Switched to First Person camera mode\n");
            }
            else if (pressed && key == '2') {
                camera.SetMode(CameraMode::ThirdPerson);
                camera.SetTarget(glm::vec3(0.0f, 0.0f, 0.0f)); // Look at origin
                camera.SetDistance(10.0f);
                OutputDebugStringA("Switched to Third Person camera mode\n");
            }
            else if (pressed && key == '3') {
                camera.SetMode(CameraMode::Orbital);
                camera.SetTarget(glm::vec3(0.0f, 0.0f, 0.0f)); // Look at origin
                camera.SetDistance(15.0f);
                OutputDebugStringA("Switched to Orbital camera mode\n");
            }
        });
        
        // Show the window
        window.Show();
        
        // Timing for frame rate and camera movement
        auto lastFrameTime = std::chrono::high_resolution_clock::now();
        
        // Main message and rendering loop
        bool running = true;
        while (running) {
            // Process window messages
            running = window.ProcessMessages();
            
            // Calculate delta time
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
            lastFrameTime = currentTime;
            
            // Limit deltaTime to prevent large jumps when debugging
            deltaTime = std::min(deltaTime, 0.1f);
            
            // Update input system
            input.Update();
            
            // Update camera
            camera.Update(&input, deltaTime);
            
            // Optional: show FPS in window title every second
            static float fpsTimer = 0.0f;
            static int frameCount = 0;
            fpsTimer += deltaTime;
            frameCount++;
            
            if (fpsTimer >= 1.0f) {
                float fps = static_cast<float>(frameCount) / fpsTimer;
                window.SetTitle(std::wstring(L"3D Game Engine - Vulkan (FPS: ") + 
                                std::to_wstring(static_cast<int>(fps)) + L")");
                fpsTimer = 0.0f;
                frameCount = 0;
            }
            
            // Rendering
            if (running) {
                try {
                    renderer.BeginFrame();
                    
                    // Set up the camera's view and projection matrices for rendering
                    renderer.SetViewMatrix(camera.GetViewMatrix());
                    renderer.SetProjectionMatrix(camera.GetProjectionMatrix());
                    
                    // Draw our triangle
                    triangleMesh->Draw(renderer.GetCommandBuffer());
                    
                    renderer.EndFrame();
                } 
                catch (const std::exception& e) {
                    OutputDebugStringA(("Error during rendering: " + std::string(e.what()) + "\n").c_str());
                }
            }
        }
        
        // Clean up resources
        MeshManager::GetInstance().Cleanup();
        
        return 0;
    }
    catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Error", MB_OK | MB_ICONERROR);
        return 1;
    }
}

