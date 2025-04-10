#include "vulkan.hpp"

// Debug messenger callback procedure
VkBool32 VKAPI_CALL Vulkan::DebugMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    printf("Validation layer: %s\n", pCallbackData->pMessage);
    return VK_FALSE;
}

Vulkan::Vulkan(System::Window* window) {

    /* VULKAN INSTANCE */

    // Define the validation layers
    const char* validation_layers[] = {
        "VK_LAYER_KHRONOS_validation"
    };

    // Define the extensions
    const char* extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    };

    // Define the application descriptor
    VkApplicationInfo application_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = "Tanx",
        .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
        .pEngineName = "Light",
        .engineVersion = VK_MAKE_VERSION(0, 1, 0),
        .apiVersion = VK_API_VERSION_1_4
    };

    // Define the Vulkan instance descriptor
    VkInstanceCreateInfo instance_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .pApplicationInfo = &application_info,
        .enabledLayerCount = 1,
        .ppEnabledLayerNames = validation_layers,
        .enabledExtensionCount = 2,
        .ppEnabledExtensionNames = extensions
    };
    
    // Create the Vulkan instance
    VK_CHECK(vkCreateInstance(&instance_info, NULL, &instance));

    /* VULKAN SURFACE */

    // Window surface (window's screen) descriptor
    VkWin32SurfaceCreateInfoKHR surface_info = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .hinstance = window->instance,
        .hwnd = window->handle
    };

    // Create the surface
    VK_CHECK(vkCreateWin32SurfaceKHR(instance, &surface_info, NULL, &surface));

    /* VULKAN DEBUG */

    // Debug messenger descriptor
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_info = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = NULL,
        .flags = 0,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = DebugMessengerCallback,
        .pUserData = NULL
    };
    
    vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    // Create the debug messenger
    vkCreateDebugUtilsMessengerEXT(instance, &debug_messenger_info, NULL, &debug_messenger);

    /* VULKAN DEVICE */

    // Enumerate phisical devices (adapters)
    uint32_t gpu_count = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(instance, &gpu_count, NULL));
    auto adapters = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * gpu_count);
    if (adapters == NULL) {
        // Handle allocation failure
        fprintf(stderr, "Failed to allocate memory for GPUs\n");
        exit(EXIT_FAILURE);
    }

    // Get the adapters
    VK_CHECK(vkEnumeratePhysicalDevices(instance, &gpu_count, adapters));

    // Select the GPU
    physical_device = adapters[0];

    // Device queue descriptor
    float queue_priority = 1.0f;
    VkDeviceQueueCreateInfo queue_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .queueFamilyIndex = 0,
        .queueCount = 1,
        .pQueuePriorities = &queue_priority
    };

    // Device extensions
    const char* device_extensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    // Logical device descriptor
    VkDeviceCreateInfo device_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = 1,
        .pQueueCreateInfos = &queue_info,
        .enabledExtensionCount = 1,
        .ppEnabledExtensionNames = device_extensions
    };

    // Create the logical device
    VK_CHECK(vkCreateDevice(physical_device, &device_info, NULL, &device));

    // Get the queue
    vkGetDeviceQueue(device, 0, 0, &queue);

    /* VULKAN SWAPCHAIN */

    extent = { (uint32)window->screen_width, (uint32)window->screen_height };

    // Create the swapchain
    VkSwapchainCreateInfoKHR swapchain_info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .surface = surface,
        .minImageCount = 2,
        .imageFormat = VK_FORMAT_B8G8R8A8_UNORM,
        .imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 0,
        .pQueueFamilyIndices = NULL,
        .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = VK_PRESENT_MODE_FIFO_KHR,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    // Create the swapchain
    VK_CHECK(vkCreateSwapchainKHR(device, &swapchain_info, NULL, &swapchain));

    // Get the swapchain images
    VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &image_count, NULL));
    image = (VkImage*)malloc(sizeof(VkImage) * image_count);
    if (image == NULL) {
        // Handle allocation failure
        fprintf(stderr, "Failed to allocate memory for swapchain images\n");
        exit(EXIT_FAILURE);
    }

    /* VULKAN IMAGE VIEWS */ // TODO: Fix this

    image_count = RENDER_TARGETS;
    VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &image_count, image));
    
    VkImageViewCreateInfo image_view_info[RENDER_TARGETS];
    for (int i = 0; i < RENDER_TARGETS; ++i) {
        image_view_info[i] = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .pNext = NULL,
            .flags = 0,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = VK_FORMAT_B8G8R8A8_UNORM,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };
    }

    // Create the image views
    image_view = (VkImageView*)malloc(sizeof(VkImageView) * image_count);
    if (image_view == NULL) {
        // Handle allocation failure
        fprintf(stderr, "Failed to allocate memory for image views\n");
        exit(EXIT_FAILURE);
    }

    vkCreateImageView(device, &image_view_info[0], NULL, &image_view[0]);
    vkCreateImageView(device, &image_view_info[1], NULL, &image_view[1]);


}

VulkanPhysicalDevices::VulkanPhysicalDevices(const VkInstance& instance, const VkSurfaceKHR& surface)
{
}
