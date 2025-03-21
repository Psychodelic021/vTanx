#pragma once

#include <intrin.h>
#include <stdio.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#include "win32.h"

#ifdef DEBUG
#define VK_CHECK(x) \
    { \
        VkResult result = x; \
        if (VK_SUCCESS != result) \
        { \
            char str[16]; \
            sprintf(str, "Vulkan error: %d", (int)result); \
            printf(str, "Vulkan error: %d", (int)result); \
            OutputDebugString(str); \
            __debugbreak(); \
        } \
    }
#else
#define VK_CHECK(x) x
#endif

struct VulkanContext {
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice GPU;
    VkDevice device;
    VkQueue queue;
    VkSwapchainKHR swapchain;
    VkImage* renderTarget;

    VulkanContext(Win32::Window* window);
};