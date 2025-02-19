#include "vk.h"

void VulkanInit(VulkanContext* context, WindowState* window) {

    // Define the validation layers
    const char* validationLayers[] = {
        "VK_LAYER_KHRONOS_validation"
    };

    // Define the extensions
    const char* extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    };

    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "vTanx",
        .pEngineName = "VNG",
    };

    VkInstanceCreateInfo instanceInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = 1,
        .ppEnabledLayerNames = validationLayers,
        .enabledExtensionCount = 2,
        .ppEnabledExtensionNames = extensions
    };
    
    VK_CHECK(vkCreateInstance(&instanceInfo, NULL, &context->instance));

    VkWin32SurfaceCreateInfoKHR surfaceInfo = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hinstance = window->instance,
        .hwnd = window->handle,
    };

    VK_CHECK(vkCreateWin32SurfaceKHR(context->instance, &surfaceInfo, NULL, &context->surface));

    
}