#pragma once

#include "common.h"
#include "system.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#pragma comment(lib, "vulkan-1.lib")

#ifdef _DEBUG
#define VK_CHECK(x) { \
    VkResult result = x; \
        if (result != VK_SUCCESS) { \
            SetConsoleColor(FOREGROUND_RED); \
            printf("[VK_CHECK ERROR] %s(%d)\n", __FILE__, __LINE__); \
            SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
            __debugbreak(); \
        } \
    }
#else
#define VK_CHECK(x) x
#endif

typedef struct Vulkan {

    Window* window;
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice gpu;
    VkPhysicalDeviceProperties gpuProps;

} Vulkan;

Vulkan VulkanInit(Window* wnd);
