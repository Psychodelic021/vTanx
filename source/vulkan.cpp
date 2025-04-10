#include <vulkan.hh>

Vulkan::Vulkan(System* system, const Settings* settings)
{
    printf("Vulkan: initializing...\n");

    pSystem = system;
    Instance = NULL;

    CreateInstance(settings->name);
    CreateSurface();
    GetPhysicalDevices();
    QFamily = SelectPhysicalDevice(VK_QUEUE_GRAPHICS_BIT, true);

    printf("Vulkan: ON\n");
}

Vulkan::~Vulkan()
{
    vkDestroySurfaceKHR(Instance, Surface, NULL);
    vkDestroyInstance(Instance, NULL);

    printf("Vulkan: OFF\n");
}

void Vulkan::CreateInstance(const char* appname)
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
        .pApplicationName = appname,
        .applicationVersion = VK_MAKE_API_VERSION(0, 0, 1, 0),
        .pEngineName = appname,
        .engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0),
        .apiVersion = VK_API_VERSION_1_3
    };

    VkInstanceCreateInfo instanceInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = NULL,
        .flags = NULL,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = std::size(layers),
        .ppEnabledLayerNames = layers,
        .enabledExtensionCount = std::size(extensions),
        .ppEnabledExtensionNames = extensions
    };

    VK_CHECK(vkCreateInstance(&instanceInfo, NULL, &Instance));
}

void Vulkan::CreateSurface()
{
    VkWin32SurfaceCreateInfoKHR surfaceInfo = {
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .hinstance = pSystem->GetInstance(),
        .hwnd = pSystem->GetWindowHandle()
    };

    VK_CHECK(vkCreateWin32SurfaceKHR(Instance, &surfaceInfo, NULL, &Surface));
}

void Vulkan::GetPhysicalDevices()
{
    uint numDevices = 0;
    VK_CHECK(vkEnumeratePhysicalDevices(Instance, &numDevices, NULL));
    PhysicalDevices.resize(numDevices);
    std::vector<VkPhysicalDevice> tempPhysicalDevices(numDevices);
    VK_CHECK(vkEnumeratePhysicalDevices(Instance, &numDevices, tempPhysicalDevices.data()));
    printf("Number of physical devices: %d\n\n", numDevices);

    for (uint i = 0; i < numDevices; i++)
    {
        PhysicalDevices[i].dev = tempPhysicalDevices[i];
        vkGetPhysicalDeviceProperties(PhysicalDevices[i].dev, &PhysicalDevices[i].devProps);
        printf("GPU[%d]: %s\n", i+1, PhysicalDevices[i].devProps.deviceName);
        uint apiVersion = PhysicalDevices[i].devProps.apiVersion;
        printf("\tVulkan API version: %d.%d.%d.%d\n", VK_API_VERSION_VARIANT(apiVersion), VK_API_VERSION_MAJOR(apiVersion),
            VK_API_VERSION_MINOR(apiVersion), VK_API_VERSION_PATCH(apiVersion));

        uint numQFamilies = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevices[i].dev, &numQFamilies, NULL);
        printf("\tNumber of queue families: %d\n", numQFamilies);
        PhysicalDevices[i].QFamilyProps.resize(numQFamilies);
        PhysicalDevices[i].QSupportsPresent.resize(numQFamilies);
        vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevices[i].dev, &numQFamilies, PhysicalDevices[i].QFamilyProps.data());

        for (uint q = 0; q < numQFamilies; q++)
        {
            printf("\tQueue family: %d, number of queues: %d\n", q, PhysicalDevices[i].QFamilyProps[q].queueCount);
            printf("\tGFX: %s, Compute: %s, Transfer: %s, Sparse binding: %s\n",
                (PhysicalDevices[i].QFamilyProps[q].queueFlags & VK_QUEUE_GRAPHICS_BIT) ? "Yes" : "No",
                (PhysicalDevices[i].QFamilyProps[q].queueFlags & VK_QUEUE_COMPUTE_BIT) ? "Yes" : "No",
                (PhysicalDevices[i].QFamilyProps[q].queueFlags & VK_QUEUE_TRANSFER_BIT) ? "Yes" : "No",
                (PhysicalDevices[i].QFamilyProps[q].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) ? "Yes" : "No");
            
            VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevices[i].dev, q, Surface,
                &(PhysicalDevices[i].QSupportsPresent[q])));

        }

        uint numFormats = 0;
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevices[i].dev, Surface, &numFormats, NULL));
        assert(numFormats > 0);
        PhysicalDevices[i].surfaceFormats.resize(numFormats);
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(PhysicalDevices[i].dev, Surface, &numFormats,
            PhysicalDevices[i].surfaceFormats.data()));

        for (uint j = 0; j < numFormats; j++)
        {
            printf("\tFormat: %x, color space: %x\n", PhysicalDevices[i].surfaceFormats[j].format,
                PhysicalDevices[i].surfaceFormats[j].colorSpace);
        }

        VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(PhysicalDevices[i].dev, Surface, &(PhysicalDevices[i].surfaceCaps)));
        // TODO: PrintImageUsageFlags(PhysicalDevices[i].surfaceCaps.supportedUsageFlags);

        uint numPresentModes = 0;
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevices[i].dev, Surface, &numPresentModes, NULL));
        assert(numPresentModes > 0);
        PhysicalDevices[i].presentModes.resize(numPresentModes);
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(PhysicalDevices[i].dev, Surface, &numPresentModes,
            PhysicalDevices[i].presentModes.data()));
        printf("\tNumber of presentation modes: %d\n", numPresentModes);
        
        vkGetPhysicalDeviceMemoryProperties(PhysicalDevices[i].dev, &(PhysicalDevices[i].memProps));
        uint numMemTypes = PhysicalDevices[i].memProps.memoryTypeCount;
        printf("\tNumber of memory types: %d\n", numMemTypes);

        for (uint j = 0; j < numMemTypes;j++)
        {
            printf("\tMemory type %d: flags: %x, heap %d\n", j,
                PhysicalDevices[i].memProps.memoryTypes[j].propertyFlags,
                PhysicalDevices[i].memProps.memoryTypes[j].heapIndex);
            // TODO: PrintMemoryProperties(PhysicalDevices[i].memProps.memoryTypes[j].propertyFlags);
        }

        printf("\tNumber of heap types: %d\n", PhysicalDevices[i].memProps.memoryHeapCount);
        printf("\n");
        
    }
}

uint Vulkan::SelectPhysicalDevice(VkQueueFlags reqQType, bool supportsPresent)
{
    for (uint i = 0; i < PhysicalDevices.size(); i++)
    {
        for (uint j = 0; j < PhysicalDevices[i].QFamilyProps.size(); j++)
        {
            VkQueueFlags qflags = PhysicalDevices[i].QFamilyProps[j].queueFlags;
            if ((qflags & reqQType) && ((bool)PhysicalDevices[i].QSupportsPresent[j] == supportsPresent))
            {
                gpuIndex = i;
                int queueFamily = j;
                printf("Using GPU[%d]: %s, queue family %d\n", gpuIndex+1, PhysicalDevices[i].devProps.deviceName, queueFamily);
                std::cout << std::endl;
                return queueFamily;
            }
        }
    }

    PRINT_ERROR("Required queue type and supports present not found\n");
    return 0;
}

const VkPhysicalDevice& Vulkan::GetSelectedPhysicalDevice() const
{
    if (gpuIndex < 0)
    {
        PRINT_ERROR("Physical device has not been selected\n");
        // TODO: return proper stuff
    }

    return PhysicalDevices[gpuIndex].dev;
}
