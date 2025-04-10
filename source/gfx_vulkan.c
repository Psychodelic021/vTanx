#include "gfx_vulkan.h"

static void VulkanCreateInstance(Vulkan* vk);
static void VulkanCreateSurface(Vulkan* vk);
static void VulkanGetPhysicalDevice(Vulkan* vk);

Vulkan VulkanInit(Window* wnd)
{
    printf("Vulkan: initializing...\n");

    Vulkan vk = {0};
    vk.window = wnd;

    VulkanCreateInstance(&vk);
    VulkanCreateSurface(&vk);
    VulkanGetPhysicalDevice(&vk);

    printf("Vulkan: ON\n");

    return vk;
}

void VulkanDestroy(Vulkan* vk)
{
    vkDestroySurfaceKHR(vk->instance, vk->surface, NULL);
    vkDestroyInstance(vk->instance, NULL);

    printf("Vulkan: OFF\n");
}

void VulkanCreateInstance(Vulkan* vk)
{
    const char* layers[] = {
        "VK_LAYER_KHRONOS_validation",
    };

    const char* extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    };

    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = "ZXEngine",
        .applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0),
        .pEngineName = "ZXEngine",
        .engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0),
        .apiVersion = VK_API_VERSION_1_3
    };

    VkInstanceCreateInfo instanceInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = ARRAYSIZE(layers),
        .ppEnabledLayerNames = layers,
        .enabledExtensionCount = ARRAYSIZE(extensions),
        .ppEnabledExtensionNames = extensions
    };

    VK_CHECK(vkCreateInstance(&instanceInfo, NULL, &vk->instance));
}

void VulkanCreateSurface(Vulkan* vk)
{
    VkWin32SurfaceCreateInfoKHR surfaceInfo = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .hinstance = vk->window->instance,
        .hwnd = vk->window->handle
    };

    VK_CHECK(vkCreateWin32SurfaceKHR(vk->instance, &surfaceInfo, NULL, &vk->surface));
}

void VulkanGetPhysicalDevice(Vulkan* vk)
{
    uint gpuCount = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(vk->instance, &gpuCount, NULL));

    VkPhysicalDevice gpuList[gpuCount];
    VK_CHECK(vkEnumeratePhysicalDevices(vk->instance, &gpuCount, gpuList));

    vk->gpu = gpuList[0];
    vkGetPhysicalDeviceProperties(vk->gpu, &vk->gpuProps);

    printf("Vulkan: GPU: %s\n", vk->gpuProps.deviceName);
   
}
