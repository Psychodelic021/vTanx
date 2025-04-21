#pragma once

/**
 * ZXEngine Main Header
 * Include this file to access the complete engine API
 */

// Engine version info
#define ZX_ENGINE_VERSION_MAJOR 0
#define ZX_ENGINE_VERSION_MINOR 1
#define ZX_ENGINE_VERSION_PATCH 0
#define ZX_ENGINE_VERSION_STRING "0.1.0"

// Forward declarations
struct Window;
struct Vulkan;

// Core includes
#include "../../source/common.h"
#include "../../source/config.h"
#include "../../source/system.h"
#include "../../source/debug.h"

// Math utilities
#include "../../source/vmath.h"

// Platform/Window management
#include "../../source/window.h"

// Renderer
#include "../../source/vulkan.h"

// Namespace for the entire engine
namespace ZX {
    // Engine initialization
    inline bool Initialize() {
        // Setup logging and other basic systems
        return true;
    }

    // Engine shutdown
    inline void Shutdown() {
        // Cleanup resources
    }
}