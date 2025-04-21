#pragma once

#include "common.h"
#include "window.h"

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#pragma comment(lib, "vulkan-1.lib")

// GPU selection score weights
#define GPU_SCORE_DISCRETE             1000
#define GPU_SCORE_INTEGRATED           500
#define GPU_SCORE_VIRTUAL              200
#define GPU_SCORE_CPU                  50

#define GPU_SCORE_API_VERSION_1_3      300
#define GPU_SCORE_API_VERSION_1_2      200
#define GPU_SCORE_API_VERSION_1_1      100

#define GPU_SCORE_GEOMETRY_SHADER      100
#define GPU_SCORE_TESSELLATION         100
#define GPU_SCORE_ANISOTROPIC          100
#define GPU_SCORE_BC_COMPRESSION       50
#define GPU_SCORE_MULTI_DRAW_INDIRECT  50
#define GPU_SCORE_PER_GB_MEMORY        1

#ifdef _DEBUG
#define VKCALL(x, msg) { \
    VkResult result = x; \
        if (result != VK_SUCCESS) { \
            SetConsoleColor(FOREGROUND_RED); \
            printf("[VULKAN ERROR] %s(%d): %s FAILED. VkResult: %u\n", __FILE__, __LINE__, msg, result); \
            SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
            __debugbreak(); \
        } \
        else { \
            printf("Vulkan: %s OK\n", msg); \
        } \
    }
#else
#define VKCALL(x, msg) x
#endif

// GPU selection preferences
typedef struct VulkanGpuPreferences {
    // Primary selection mode
    enum {
        GPU_SELECT_HIGHEST_PERFORMANCE,  // Favor performance (default)
        GPU_SELECT_POWER_SAVING,         // Favor power efficiency
        GPU_SELECT_SPECIFIC_NAME         // Select specific GPU by name
    } selectionMode;
    
    // Required features
    bool requireGeometryShader;
    bool requireTessellation;
    bool requireAnisotropicFiltering;
    bool requireComputeShader;
    
    // For specific GPU selection
    const char* preferredGpuName;
    
    // Weight multipliers (can be adjusted to prioritize different aspects)
    float discreteGpuMultiplier;    // Default: 1.0
    float memoryMultiplier;         // Default: 1.0
    float featureMultiplier;        // Default: 1.0
    float apiVersionMultiplier;     // Default: 1.0
    
} VulkanGpuPreferences;

// Preferred presentation mode for swapchain
typedef enum VulkanPresentMode {
    VULKAN_PRESENT_MODE_FIFO,           // V-Sync (vertical synchronization)
    VULKAN_PRESENT_MODE_MAILBOX,        // Triple buffering (if available)
    VULKAN_PRESENT_MODE_IMMEDIATE,      // Immediate mode (potential tearing)
    VULKAN_PRESENT_MODE_FIFO_RELAXED    // Relaxed V-Sync
} VulkanPresentMode;

// Initializes VulkanGpuPreferences with default values
void VulkanInitDefaultGpuPreferences(VulkanGpuPreferences* prefs);

typedef struct Vulkan {

    Window* window;
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice gpu;
    VkDevice device;
    
    // Queue family indices
    uint graphicsQueueFamily;
    uint presentQueueFamily;
    
    // Queue handles
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    
    // GPU selection preferences
    VulkanGpuPreferences gpuPreferences;
    
    // Swapchain
    VkSwapchainKHR swapchain;
    VkFormat swapchainImageFormat;
    VkExtent2D swapchainExtent;
    uint32_t swapchainImageCount;
    VkImage* swapchainImages;
    VkImageView* swapchainImageViews;

} Vulkan;

void VulkanInit(Vulkan* vk, Window* wnd);
bool VulkanCreateInstance(Vulkan* vk, const char* appName, uint32_t appVersion);
bool VulkanCreateSurface(Vulkan* vk, Window* wnd);
bool VulkanSelectPhysicalDevice(Vulkan* vk);
bool VulkanCreateLogicalDevice(Vulkan* vk, const VkPhysicalDeviceFeatures* enabledFeatures);
bool VulkanCreateSwapchain(Vulkan* vk, VulkanPresentMode preferredPresentMode);
bool VulkanRecreateSwapchain(Vulkan* vk, VulkanPresentMode preferredPresentMode);

void VulkanDestroy(Vulkan* vk);
void VulkanDestroySwapchain(Vulkan* vk);
