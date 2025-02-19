#pragma once

#include <intrin.h>
#include <stdio.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#include "win32.h"

#define VK_CHECK(x)                                        \
    {                                                      \
        VkResult result = x;                               \
        if (VK_SUCCESS != result)                          \
        {                                                  \
            char str[16];                                  \
            sprintf(str, "Vulkan error: %d", (int)result); \
            printf(str, "Vulkan error: %d", (int)result);  \
            OutputDebugString(str);                        \
            __debugbreak();                                \
        }                                                  \
    }

typedef struct {
    VkInstance instance;
    VkSurfaceKHR surface;
} VulkanContext;

void VulkanInit(VulkanContext* context, WindowState* window); 