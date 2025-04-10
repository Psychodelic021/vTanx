#pragma once

#include <common.h>
#include <system.h>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#pragma comment(lib, "vulkan-1.lib")

#ifdef _DEBUG
#define VK_CHECK(x) { \
    VkResult result = x; \
        if (result != VK_SUCCESS) { \
            System::SetConsoleColor(FOREGROUND_RED); \
            printf("[VK_CHECK ERROR] %s(%d)\n", __FILE__, __LINE__); \
            System::SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
            throw std::exception(); \
        } \
    }
#else
#define VK_CHECK(x) x
#endif

struct GPU {

    VkPhysicalDevice dev;
    VkPhysicalDeviceProperties devProps;
    std::vector<VkQueueFamilyProperties> QFamilyProps;
    std::vector<VkBool32> QSupportsPresent;
    std::vector<VkSurfaceFormatKHR> surfaceFormats;
    std::vector<VkPresentModeKHR> presentModes;
    VkSurfaceCapabilitiesKHR surfaceCaps;
    VkPhysicalDeviceMemoryProperties memProps;
};

class Vulkan {

    System* pSystem;
    VkInstance Instance;
    VkSurfaceKHR Surface;
    std::vector<GPU> PhysicalDevices;
    int gpuIndex = -1;
    uint QFamily = 0;

    public:
        Vulkan(System* system, const Settings* settings);
        ~Vulkan();

    private:
        void CreateInstance(const char* appname);
        void CreateSurface();
        void GetPhysicalDevices();
        uint SelectPhysicalDevice(VkQueueFlags requiredQueueType, bool supportsPresent);
        const VkPhysicalDevice& GetSelectedPhysicalDevice() const;
};