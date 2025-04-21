#include "vulkan.h"

// Helper function to score a physical device based on its properties and features
static int ScorePhysicalDevice(VkPhysicalDevice device, VkPhysicalDeviceProperties properties,
                             VkPhysicalDeviceFeatures features,
                             VkPhysicalDeviceMemoryProperties memory,
                             const VulkanGpuPreferences* prefs)
{
    // Check for required features first
    if (prefs->requireGeometryShader && !features.geometryShader) {
        return -1;  // Disqualify this GPU
    }
    
    if (prefs->requireTessellation && !features.tessellationShader) {
        return -1;  // Disqualify this GPU
    }
    
    if (prefs->requireAnisotropicFiltering && !features.samplerAnisotropy) {
        return -1;  // Disqualify this GPU
    }
    
    if (prefs->requireComputeShader && !(VK_QUEUE_COMPUTE_BIT & 1)) {
        // This is a simplification - you'd need to check queue properties more thoroughly
        return -1;
    }
    
    // Check for specifically named GPU if that mode is active
    if (prefs->selectionMode == GPU_SELECT_SPECIFIC_NAME && 
        prefs->preferredGpuName != NULL) {
        // If we're looking for a specific GPU and this matches, give it max score
        if (strcmp(properties.deviceName, prefs->preferredGpuName) == 0) {
            return INT_MAX;  // Maximum possible score
        }
    }

    int score = 0;
    
    // Apply different scoring strategies based on selection mode
    if (prefs->selectionMode == GPU_SELECT_POWER_SAVING) {
        // For power saving mode, we actually prefer integrated GPUs
        switch (properties.deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                score += (int)(GPU_SCORE_DISCRETE * prefs->discreteGpuMultiplier);
                break;
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                score += (int)(GPU_SCORE_INTEGRATED * prefs->discreteGpuMultiplier);
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                score += (int)(GPU_SCORE_VIRTUAL * prefs->discreteGpuMultiplier);
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                score += (int)(GPU_SCORE_CPU * prefs->discreteGpuMultiplier);
                break;
            default:
                break;
        }
    } else {
        // Default high-performance mode
        switch (properties.deviceType) {
            case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
                score += (int)(GPU_SCORE_DISCRETE * prefs->discreteGpuMultiplier);
                break;
            case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
                score += (int)(GPU_SCORE_INTEGRATED * prefs->discreteGpuMultiplier);
                break;
            case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
                score += (int)(GPU_SCORE_VIRTUAL * prefs->discreteGpuMultiplier);
                break;
            case VK_PHYSICAL_DEVICE_TYPE_CPU:
                score += (int)(GPU_SCORE_CPU * prefs->discreteGpuMultiplier);
                break;
            default:
                break;
        }
    }
    
    // Score based on device local memory amount (points per GB)
    VkDeviceSize total_memory = 0;
    for (uint j = 0; j < memory.memoryHeapCount; j++) {
        if (memory.memoryHeaps[j].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
            total_memory += memory.memoryHeaps[j].size;
        }
    }
    score += (int)((total_memory / (1024 * 1024 * 1024)) * GPU_SCORE_PER_GB_MEMORY * prefs->memoryMultiplier);
    
    // Add points for API version support with preference multiplier
    if (properties.apiVersion >= VK_API_VERSION_1_3) {
        score += (int)(GPU_SCORE_API_VERSION_1_3 * prefs->apiVersionMultiplier);
    } else if (properties.apiVersion >= VK_API_VERSION_1_2) {
        score += (int)(GPU_SCORE_API_VERSION_1_2 * prefs->apiVersionMultiplier);
    } else if (properties.apiVersion >= VK_API_VERSION_1_1) {
        score += (int)(GPU_SCORE_API_VERSION_1_1 * prefs->apiVersionMultiplier);
    }
    
    // Add points for important features with preference multiplier
    float feature_score = 0.0f;
    if (features.geometryShader)      feature_score += GPU_SCORE_GEOMETRY_SHADER;
    if (features.tessellationShader)  feature_score += GPU_SCORE_TESSELLATION;
    if (features.samplerAnisotropy)   feature_score += GPU_SCORE_ANISOTROPIC;
    if (features.textureCompressionBC) feature_score += GPU_SCORE_BC_COMPRESSION;
    if (features.multiDrawIndirect)   feature_score += GPU_SCORE_MULTI_DRAW_INDIRECT;
    
    score += (int)(feature_score * prefs->featureMultiplier);
    
    return score;
}

// Helper function to find queue families on a physical device
static bool FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface, 
                            uint* graphicsQueueFamily, uint* presentQueueFamily)
{
    *graphicsQueueFamily = UINT_MAX;
    *presentQueueFamily = UINT_MAX;
    
    uint queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);
    
    VkQueueFamilyProperties queue_families[queue_family_count];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);
    
    // Look for graphics and presentation support
    for (uint i = 0; i < queue_family_count; i++) {
        // Check for graphics support
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            *graphicsQueueFamily = i;
        }
        
        // Check for presentation support
        VkBool32 present_support = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);
        
        if (present_support) {
            *presentQueueFamily = i;
        }
        
        // If we found both queues, we can stop searching
        if (*graphicsQueueFamily != UINT_MAX && *presentQueueFamily != UINT_MAX) {
            break;
        }
    }
    
    // To be suitable, the device must support both graphics and presentation
    return (*graphicsQueueFamily != UINT_MAX && *presentQueueFamily != UINT_MAX);
}

// Print detailed device capabilities to help with debugging
static void PrintDeviceCapabilities(VkPhysicalDevice device, VkPhysicalDeviceProperties props, 
                                 VkPhysicalDeviceFeatures features, uint device_index)
{
    // Get device type as string
    const char* device_type_str = "Unknown";
    switch (props.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: device_type_str = "Integrated GPU"; break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: device_type_str = "Discrete GPU"; break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: device_type_str = "Virtual GPU"; break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU: device_type_str = "CPU"; break;
        default: break;
    }
    
    // Print basic device information
    PRINT_TRACE("Vulkan: GPU %u: %s (Type: %s)\n", device_index, props.deviceName, device_type_str);
    PRINT("  - API Version: %u.%u.%u\n", 
        VK_VERSION_MAJOR(props.apiVersion),
        VK_VERSION_MINOR(props.apiVersion),
        VK_VERSION_PATCH(props.apiVersion));
    PRINT("  - Driver Version: %u.%u.%u\n", 
        VK_VERSION_MAJOR(props.driverVersion),
        VK_VERSION_MINOR(props.driverVersion),
        VK_VERSION_PATCH(props.driverVersion));
    
    // Print key feature support
    PRINT("  - Key Features: %s%s%s%s\n",
        features.geometryShader ? "Geometry " : "",
        features.tessellationShader ? "Tessellation " : "",
        features.samplerAnisotropy ? "Anisotropy " : "",
        features.textureCompressionBC ? "BC-Compression " : "");
}

bool VulkanSelectPhysicalDevice(Vulkan* vk)
{
    // Select physical device (GPU) -------------------------------------------
    
    uint gpu_count = 0;
    VKCALL(vkEnumeratePhysicalDevices(vk->instance, &gpu_count, NULL), "vkEnumeratePhysicalDevices(count)");
    
    if (gpu_count == 0) {
        PRINT_ERROR("Vulkan: No GPUs with Vulkan support found\n");
        return false;
    }

    VkPhysicalDevice GPU[gpu_count];
    VKCALL(vkEnumeratePhysicalDevices(vk->instance, &gpu_count, GPU), "vkEnumeratePhysicalDevices(data)");

    // GPU selection criteria
    int selected_gpu_index = -1;
    int best_score = -1;

    PRINT("Vulkan: Found %u GPUs\n", gpu_count);

    for (uint i = 0; i < gpu_count; i++) {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(GPU[i], &properties);
        
        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(GPU[i], &features);
        
        VkPhysicalDeviceMemoryProperties memory;
        vkGetPhysicalDeviceMemoryProperties(GPU[i], &memory);

        // Print detailed capabilities for debugging
        PrintDeviceCapabilities(GPU[i], properties, features, i);

        // First check if the device supports the required queue families
        uint graphics_queue_family, present_queue_family;
        bool has_required_queue_support = FindQueueFamilies(GPU[i], vk->surface, 
                                                      &graphics_queue_family, 
                                                      &present_queue_family);

        if (has_required_queue_support) {
            // Calculate score for this device
            int score = ScorePhysicalDevice(GPU[i], properties, features, memory, &vk->gpuPreferences);
            PRINT("Vulkan: GPU %u score: %d\n", i, score);
            
            if (score > best_score) {
                best_score = score;
                selected_gpu_index = i;
            }
        } else {
            PRINT_WARNING("Vulkan: GPU %u doesn't support required queue families\n", i);
        }
    }

    // If we found a suitable GPU, set it up
    if (selected_gpu_index >= 0) {
        vk->gpu = GPU[selected_gpu_index];
        
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(vk->gpu, &properties);
        
        PRINT_INFO("Vulkan: Selected GPU: %s\n", properties.deviceName);
        
        // Find queue family indices for the selected GPU
        bool found_queues = FindQueueFamilies(vk->gpu, vk->surface, 
                                          &vk->graphicsQueueFamily, 
                                          &vk->presentQueueFamily);
        
        if (!found_queues) {
            PRINT_ERROR("Vulkan: Failed to find queue families on selected GPU\n");
            return false;
        }
        
        PRINT("Vulkan: Graphics queue family: %u\n", vk->graphicsQueueFamily);
        PRINT("Vulkan: Present queue family: %u\n", vk->presentQueueFamily);
        return true;
    } else {
        PRINT_ERROR("Vulkan: Failed to find a suitable GPU\n");
        return false;
    }
}

void VulkanInit(Vulkan* vk, Window* wnd)
{
    PRINT_DEBUG("Vulkan: initializing...\n");
    
    // Initialize default GPU preferences
    VulkanInitDefaultGpuPreferences(&vk->gpuPreferences);

    // Create vulkan instance
    if (!VulkanCreateInstance(vk, "ZXEngine", VK_MAKE_VERSION(0, 1, 0))) {
        PRINT("Vulkan: Failed to create instance\n");
        return;
    }

    // Create rendering surface
    if (!VulkanCreateSurface(vk, wnd)) {
        PRINT("Vulkan: Failed to create window surface\n");
        return;
    }

    // Select physical device (GPU)
    if (!VulkanSelectPhysicalDevice(vk)) {
        PRINT("Vulkan: Failed to find a suitable GPU\n");
        return;
    }
    
    // Create logical device with default features
    if (!VulkanCreateLogicalDevice(vk, NULL)) {
        PRINT("Vulkan: Failed to create logical device\n");
        return;
    }
    
    // Create swapchain - using mailbox mode (triple buffering) if available
    if (!VulkanCreateSwapchain(vk, VULKAN_PRESENT_MODE_MAILBOX)) {
        PRINT("Vulkan: Failed to create swapchain\n");
        return;
    }
    
    PRINT_INFO("Vulkan: ON\n");
}



void VulkanInitDefaultGpuPreferences(VulkanGpuPreferences* prefs)
{
    // Set default selection mode to highest performance
    prefs->selectionMode = GPU_SELECT_HIGHEST_PERFORMANCE;
    
    // Default required features (not required by default)
    prefs->requireGeometryShader = false;
    prefs->requireTessellation = false;
    prefs->requireAnisotropicFiltering = false;
    prefs->requireComputeShader = false;
    
    // No preferred GPU by default
    prefs->preferredGpuName = NULL;
    
    // Default weight multipliers
    prefs->discreteGpuMultiplier = 1.0f;
    prefs->memoryMultiplier = 1.0f;
    prefs->featureMultiplier = 1.0f;
    prefs->apiVersionMultiplier = 1.0f;
}

// Check if validation layers are supported
static bool CheckValidationLayerSupport(const char** requested_layers, uint32_t layer_count) {
    uint32_t available_layer_count;
    VKCALL(vkEnumerateInstanceLayerProperties(&available_layer_count, NULL),
        "vkEnumerateInstanceLayerProperties(count)");
    
    VkLayerProperties available_layers[available_layer_count];
    VKCALL(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers),
        "vkEnumerateInstanceLayerProperties(layers)");
    
    // Check if each requested layer is in the available layers
    for (uint32_t i = 0; i < layer_count; i++) {
        bool layer_found = false;
        
        for (uint32_t j = 0; j < available_layer_count; j++) {
            if (strcmp(requested_layers[i], available_layers[j].layerName) == 0) {
                layer_found = true;
                break;
            }
        }
        
        if (!layer_found) {
            PRINT_WARNING("Validation layer %s not available\n", requested_layers[i]);
            return false;
        }
    }
    
    return true;
}

// Get required instance extensions
static void GetRequiredExtensions(const char*** extensions, uint32_t* extension_count) {
    // Base extensions needed
    static const char* base_extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
    #ifdef _WIN32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    #endif
    };
    
    // Debug extensions
    #ifdef _DEBUG
    static const char* debug_extensions[] = {
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        VK_KHR_SURFACE_EXTENSION_NAME,
    #ifdef _WIN32
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    #endif
    };
    
    *extensions = debug_extensions;
    *extension_count = sizeof(debug_extensions) / sizeof(debug_extensions[0]);
    #else
    *extensions = base_extensions;
    *extension_count = sizeof(base_extensions) / sizeof(base_extensions[0]);
    #endif
}

bool VulkanCreateInstance(Vulkan* vk, const char* app_name, uint32_t app_version) {
    // Determine layers and extensions
    const char* validation_layers[] = {
        "VK_LAYER_KHRONOS_validation"
    };
    uint32_t validation_layer_count = 0;
    
    #ifdef _DEBUG
    validation_layer_count = sizeof(validation_layers) / sizeof(validation_layers[0]);
    
    // Verify validation layer support
    if (validation_layer_count > 0) {
        if (!CheckValidationLayerSupport(validation_layers, validation_layer_count)) {
            PRINT("Warning: Requested validation layers not available, proceeding without validation\n");
            validation_layer_count = 0; // Disable validation if not supported
        }
    }
    #endif
    
    // Get required extensions
    const char** extensions;
    uint32_t extension_count;
    GetRequiredExtensions(&extensions, &extension_count);
    
    // Application info
    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pNext = NULL,
        .pApplicationName = app_name ? app_name : "ZXEngine",
        .applicationVersion = app_version ? app_version : VK_MAKE_VERSION(0, 1, 0),
        .pEngineName = "ZXEngine",
        .engineVersion = VK_MAKE_VERSION(0, 1, 0),
        .apiVersion = VK_API_VERSION_1_2
    };
    
    // Instance create info
    VkInstanceCreateInfo instance_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = validation_layer_count,
        .ppEnabledLayerNames = validation_layer_count > 0 ? validation_layers : NULL,
        .enabledExtensionCount = extension_count,
        .ppEnabledExtensionNames = extensions
    };
    
    // Create the instance
    VKCALL(vkCreateInstance(&instance_info, NULL, &vk->instance), "vkCreateInstance");
    
    // Additional debug features could be set up here in the future
    // (Debug messenger, etc.)
    
    return true;
}

bool VulkanCreateSurface(Vulkan* vk, Window* wnd)
{
    // Store window pointer for later use
    vk->window = wnd;

    #ifdef _WIN32
    // Create Win32 surface
    VkWin32SurfaceCreateInfoKHR surface_info = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .hinstance = wnd->instance,
        .hwnd = wnd->handle
    };
    
    VKCALL(vkCreateWin32SurfaceKHR(vk->instance, &surface_info, NULL, &vk->surface), "vkCreateWin32SurfaceKHR");
    
    #else
    #error "Platform not supported"
    #endif

    return true;
}

bool VulkanCreateLogicalDevice(Vulkan* vk, const VkPhysicalDeviceFeatures* enabled_features)
{
    // Check if we have valid queue family indices
    if (vk->graphicsQueueFamily == UINT_MAX || vk->presentQueueFamily == UINT_MAX) {
        PRINT_ERROR("Vulkan: Invalid queue family indices for logical device creation\n");
        return false;
    }

    // Setup queue create infos
    const float queue_priority = 1.0f;
    VkDeviceQueueCreateInfo queue_create_infos[2];
    uint queue_create_info_count = 0;
    
    // Always add graphics queue
    VkDeviceQueueCreateInfo graphics_queue_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = vk->graphicsQueueFamily,
        .queueCount = 1,
        .pQueuePriorities = &queue_priority
    };
    queue_create_infos[queue_create_info_count++] = graphics_queue_info;
    
    // Add present queue only if it differs from graphics queue
    if (vk->presentQueueFamily != vk->graphicsQueueFamily) {
        VkDeviceQueueCreateInfo present_queue_info = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = vk->presentQueueFamily,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority
        };
        queue_create_infos[queue_create_info_count++] = present_queue_info;
    }
    
    // Setup device features
    VkPhysicalDeviceFeatures device_features = {0};
    if (enabled_features) {
        // Use provided features if available
        device_features = *enabled_features;
    } else {
        // Otherwise enable some commonly useful features
        VkPhysicalDeviceFeatures available_features;
        vkGetPhysicalDeviceFeatures(vk->gpu, &available_features);
        
        // Enable anisotropic filtering if available
        if (available_features.samplerAnisotropy) {
            device_features.samplerAnisotropy = VK_TRUE;
        }
        
        // Enable texture compression formats if available
        if (available_features.textureCompressionBC) {
            device_features.textureCompressionBC = VK_TRUE;
        }
    }
    
    // Required device extensions
    const char* device_extensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME  // Swapchain is required for presenting to surfaces
    };
    
    // Create the logical device
    VkDeviceCreateInfo device_create_info = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = queue_create_info_count,
        .pQueueCreateInfos = queue_create_infos,
        .enabledExtensionCount = ARRAYSIZE(device_extensions),
        .ppEnabledExtensionNames = device_extensions,
        .pEnabledFeatures = &device_features
    };
    
    VKCALL(vkCreateDevice(vk->gpu, &device_create_info, NULL, &vk->device), "vkCreateDevice");
    
    // Get queue handles
    vkGetDeviceQueue(vk->device, vk->graphicsQueueFamily, 0, &vk->graphicsQueue);
    vkGetDeviceQueue(vk->device, vk->presentQueueFamily, 0, &vk->presentQueue);
    
    PRINT("Vulkan: Logical device created successfully\n");
    return true;
}

// Choose the best presentation mode based on user preference
static VkPresentModeKHR ChooseSwapPresentMode(const VkPresentModeKHR* available_present_modes, 
                                           uint32_t present_mode_count, 
                                           VulkanPresentMode preferred_mode) {
    // Default to FIFO (V-sync) as fallback - guaranteed to be available
    VkPresentModeKHR result = VK_PRESENT_MODE_FIFO_KHR;
    
    // Select desired present mode based on preference
    VkPresentModeKHR desired_mode;
    switch (preferred_mode) {
        case VULKAN_PRESENT_MODE_MAILBOX:
            desired_mode = VK_PRESENT_MODE_MAILBOX_KHR;
            break;
        case VULKAN_PRESENT_MODE_IMMEDIATE:
            desired_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
            break;
        case VULKAN_PRESENT_MODE_FIFO_RELAXED:
            desired_mode = VK_PRESENT_MODE_FIFO_RELAXED_KHR;
            break;
        case VULKAN_PRESENT_MODE_FIFO:
        default:
            // No need to search, just return FIFO
            return VK_PRESENT_MODE_FIFO_KHR;
    }
    
    // Check if the desired mode is available
    for (uint32_t i = 0; i < present_mode_count; i++) {
        if (available_present_modes[i] == desired_mode) {
            return desired_mode;
        }
    }
    
    // If we reach here, the desired mode wasn't available, so return FIFO
    return result;
}

// Choose the best surface format (color depth and space)
static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const VkSurfaceFormatKHR* available_formats, uint32_t format_count) {
    // Preferred: SRGB color space with B8G8R8A8 format
    for (uint32_t i = 0; i < format_count; i++) {
        if (available_formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && 
            available_formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return available_formats[i];
        }
    }
    
    // If preferred format isn't available, just use the first one
    return available_formats[0];
}

// Choose the swap extent (resolution of the swapchain images)
static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR* capabilities, Window* window) {
    if (capabilities->currentExtent.width != UINT_MAX) {
        // If the surface size is defined, use that
        return capabilities->currentExtent;
    } else {
        // Otherwise use the window size clamped to the min/max extents
        VkExtent2D actual_extent = {
            (uint32_t)window->width,
            (uint32_t)window->height
        };
        
        actual_extent.width = MAX(capabilities->minImageExtent.width, 
                              MIN(capabilities->maxImageExtent.width, actual_extent.width));
        actual_extent.height = MAX(capabilities->minImageExtent.height, 
                               MIN(capabilities->maxImageExtent.height, actual_extent.height));
        
        return actual_extent;
    }
}

bool VulkanCreateSwapchain(Vulkan* vk, VulkanPresentMode preferred_mode) {
    // 1. Query details of the surface
    VkSurfaceCapabilitiesKHR surface_capabilities;
    VKCALL(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk->gpu, vk->surface, &surface_capabilities),
           "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
    
    uint32_t format_count;
    VKCALL(vkGetPhysicalDeviceSurfaceFormatsKHR(vk->gpu, vk->surface, &format_count, NULL), "vkGetPhysicalDeviceSurfaceFormatsKHR");
    
    if (format_count == 0) {
        PRINT("Vulkan: No surface formats supported\n");
        return false;
    }
    
    VkSurfaceFormatKHR surface_formats[format_count];
    VKCALL(vkGetPhysicalDeviceSurfaceFormatsKHR(vk->gpu, vk->surface, &format_count, surface_formats),
           "vkGetPhysicalDeviceSurfaceFormatsKHR");
    
    uint32_t present_mode_count;
    VKCALL(vkGetPhysicalDeviceSurfacePresentModesKHR(vk->gpu, vk->surface, &present_mode_count, NULL),
        "vkGetPhysicalDeviceSurfacePresentModesKHR(count)");
    
    if (present_mode_count == 0) {
        PRINT("Vulkan: No surface present modes supported\n");
        return false;
    }
    
    VkPresentModeKHR present_modes[present_mode_count];
    VKCALL(vkGetPhysicalDeviceSurfacePresentModesKHR(vk->gpu, vk->surface, &present_mode_count, present_modes),
           "vkGetPhysicalDeviceSurfacePresentModesKHR(data)");
    
    // 2. Choose the optimal settings
    VkSurfaceFormatKHR surface_format = ChooseSwapSurfaceFormat(surface_formats, format_count);
    VkPresentModeKHR present_mode = ChooseSwapPresentMode(present_modes, present_mode_count, preferred_mode);
    VkExtent2D extent = ChooseSwapExtent(&surface_capabilities, vk->window);
    
    // Log which presentation mode was selected
    const char* present_mode_name;
    switch (present_mode) {
        case VK_PRESENT_MODE_MAILBOX_KHR:
            present_mode_name = "MAILBOX (Triple Buffering)";
            break;
        case VK_PRESENT_MODE_IMMEDIATE_KHR:
            present_mode_name = "IMMEDIATE (Uncapped FPS, may tear)";
            break;
        case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
            present_mode_name = "FIFO RELAXED (Adaptive V-Sync)";
            break;
        case VK_PRESENT_MODE_FIFO_KHR:
            present_mode_name = "FIFO (V-Sync)";
            break;
        default:
            present_mode_name = "Unknown";
    }
    PRINT("Vulkan: Using presentation mode: %s\n", present_mode_name);
    
    // Request one more image than the minimum to avoid waiting for the driver
    uint32_t image_count = surface_capabilities.minImageCount + 1;
    
    // Make sure not to exceed the maximum number of images
    if (surface_capabilities.maxImageCount > 0 && image_count > surface_capabilities.maxImageCount) {
        image_count = surface_capabilities.maxImageCount;
    }
    
    // 3. Create the swapchain
    VkSwapchainCreateInfoKHR swapchain_create_info = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = vk->surface,
        .minImageCount = image_count,
        .imageFormat = surface_format.format,
        .imageColorSpace = surface_format.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1, // More for stereoscopic 3D
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = surface_capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };
    
    // Handle queue family sharing
    if (vk->graphicsQueueFamily != vk->presentQueueFamily) {
        // If graphics and present queue are different, we need concurrent sharing
        uint32_t queue_family_indices[] = { vk->graphicsQueueFamily, vk->presentQueueFamily };
        
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_create_info.queueFamilyIndexCount = 2;
        swapchain_create_info.pQueueFamilyIndices = queue_family_indices;
    } else {
        // Otherwise, exclusive mode offers better performance
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchain_create_info.queueFamilyIndexCount = 0;
        swapchain_create_info.pQueueFamilyIndices = NULL;
    }
    
    // Create the swapchain
    VKCALL(vkCreateSwapchainKHR(vk->device, &swapchain_create_info, NULL, &vk->swapchain),
           "vkCreateSwapchainKHR");
    
    // Store swapchain format and extent
    vk->swapchainImageFormat = surface_format.format;
    vk->swapchainExtent = extent;
    
    // 4. Retrieve swapchain images
    vkGetSwapchainImagesKHR(vk->device, vk->swapchain, &vk->swapchainImageCount, NULL);
    vk->swapchainImages = malloc(vk->swapchainImageCount * sizeof(VkImage));
    
    VKCALL(vkGetSwapchainImagesKHR(vk->device, vk->swapchain, &vk->swapchainImageCount, vk->swapchainImages),
           "vkGetSwapchainImagesKHR");
    
    // 5. Create image views for all swapchain images
    vk->swapchainImageViews = malloc(vk->swapchainImageCount * sizeof(VkImageView));
    
    for (uint32_t i = 0; i < vk->swapchainImageCount; i++) {
        VkImageViewCreateInfo image_view_info = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = vk->swapchainImages[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = vk->swapchainImageFormat,
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
        
        VKCALL(vkCreateImageView(vk->device, &image_view_info, NULL, &vk->swapchainImageViews[i]),
               "vkCreateImageView");
    }
    
    PRINT("Vulkan: Created swapchain with %u images, format %d, extent %dx%d\n", 
               vk->swapchainImageCount, vk->swapchainImageFormat, 
               vk->swapchainExtent.width, vk->swapchainExtent.height);
    
    return true;
}

void VulkanDestroySwapchain(Vulkan* vk)
{
    // Destroy all image views
    if (vk->swapchainImageViews) {
        for (uint32_t i = 0; i < vk->swapchainImageCount; i++) {
            if (vk->swapchainImageViews[i]) {
                vkDestroyImageView(vk->device, vk->swapchainImageViews[i], NULL);
            }
        }
        free(vk->swapchainImageViews);
        vk->swapchainImageViews = NULL;
    }
    
    // Free swapchain images array (the images themselves are owned by the swapchain)
    if (vk->swapchainImages) {
        free(vk->swapchainImages);
        vk->swapchainImages = NULL;
    }
    
    // Destroy the swapchain
    if (vk->swapchain) {
        vkDestroySwapchainKHR(vk->device, vk->swapchain, NULL);
        vk->swapchain = VK_NULL_HANDLE;
    }
    
    PRINT("Vulkan: Swapchain destroyed\n");
}

bool VulkanRecreateSwapchain(Vulkan* vk, VulkanPresentMode preferredMode) {
    // Wait for the device to be idle before recreating the swapchain
    vkDeviceWaitIdle(vk->device);
    
    PRINT("Vulkan: Recreating swapchain due to window resize\n");
    
    // First, clean up old swapchain resources
    VulkanDestroySwapchain(vk);
    
    // Then create a new swapchain
    return VulkanCreateSwapchain(vk, preferredMode);
}

void VulkanDestroy(Vulkan* vk)
{
    // Clean up swapchain resources first
    VulkanDestroySwapchain(vk);

    // Device needs to be destroyed before the instance
    if (vk->device) {
        vkDestroyDevice(vk->device, NULL);
        vk->device = NULL;
    }

    if (vk->surface) {
        vkDestroySurfaceKHR(vk->instance, vk->surface, NULL);
        vk->surface = NULL;
    }
    
    if (vk->instance) {
        vkDestroyInstance(vk->instance, NULL);
        vk->instance = NULL;
    }

    PRINT("Vulkan: Resources destroyed\n");
}

