#include "vk.h"

Vulkan::Vulkan(System::Window* window) {

    // Define the validation layers
    const char* validationLayers[] = {
        "VK_LAYER_KHRONOS_validation"
    };

    // Define the extensions
    const char* extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    };

    // Define the application descriptor
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "vTanx",
        .pEngineName = "VNG",
        .apiVersion = VK_API_VERSION_1_4
    };

    // Define the Vulkan instance descriptor
    VkInstanceCreateInfo instanceInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = 1,
        .ppEnabledLayerNames = validationLayers,
        .enabledExtensionCount = 2,
        .ppEnabledExtensionNames = extensions
    };
    
    // Create the Vulkan instance
    VK_CHECK(vkCreateInstance(&instanceInfo, NULL, &instance));

    VkWin32SurfaceCreateInfoKHR surfaceInfo = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hinstance = window->instance,
        .hwnd = window->handle,
    };

    // Create the surface (window's screen)
    VK_CHECK(vkCreateWin32SurfaceKHR(instance, &surfaceInfo, NULL, &surface));

    // Enumerate phisical devices (adapters)
    uint32_t count = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(instance, &count, NULL));
    auto adapters = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * count);
    if (adapters == NULL) {
        // Handle allocation failure
        fprintf(stderr, "Failed to allocate memory for GPUs\n");
        exit(EXIT_FAILURE);
    }

    VK_CHECK(vkEnumeratePhysicalDevices(instance, &count, adapters));

    // Select the GPU
    physical_device = adapters[0];

    // Create a logial device
    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = 0,
        .queueCount = 1,
        .pQueuePriorities = &queuePriority
    };

    const char* deviceExtensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDeviceCreateInfo deviceInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queueInfo,
        .enabledExtensionCount = 1,
        .ppEnabledExtensionNames = deviceExtensions
    };

    VK_CHECK(vkCreateDevice(physical_device, &deviceInfo, NULL, &device));

    // Get the queue
    vkGetDeviceQueue(device, 0, 0, &queue);

    // Create the swapchain
    VkSwapchainCreateInfoKHR swapchainInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = 2,
        .imageFormat = VK_FORMAT_B8G8R8A8_UNORM,
        .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent = { window->screen_width, window->screen_height },
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR,
        .clipped = VK_TRUE
    };

    VK_CHECK(vkCreateSwapchainKHR(device, &swapchainInfo, NULL, &swapchain));

    // Get the swapchain images
    VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &count, NULL));
    image = (VkImage*)malloc(sizeof(VkImage) * count);
    if (image == NULL) {
        // Handle allocation failure
        fprintf(stderr, "Failed to allocate memory for swapchain images\n");
        exit(EXIT_FAILURE);
    }

    VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &count, image));

}