#pragma once

#include <intrin.h>
#include <stdio.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#include "..\system.h"

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

#define RENDER_TARGETS 2

struct Vulkan {

    VkInstance instance;
    VkSurfaceKHR surface;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice physical_device;
    VkDevice device;
    VkQueue queue;
    VkExtent2D extent;
    VkSwapchainKHR swapchain;
    uint32 image_count = RENDER_TARGETS;
    VkImage* image;
    VkFormat format;

    VkImageView* image_view;
    VkFramebuffer* framebuffer;
    VkRenderPass render_pass;
    VkPipelineLayout pipeline_layout;
    VkPipeline pipeline;
    VkCommandPool command_pool;
    VkCommandBuffer* command_buffer;
    VkSemaphore image_available_semaphore;
    VkSemaphore render_finished_semaphore;


    Vulkan(System::Window* window);

    static VkBool32 VKAPI_CALL DebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
};