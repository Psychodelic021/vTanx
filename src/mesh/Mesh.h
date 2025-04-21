#pragma once

#include <vector>
#include <memory>
#include <map>
#include <string>
#include <array>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

// Forward declaration
class VulkanRenderer;

// Define vertex structure
struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 color;
    
    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
        return bindingDescription;
    }
    
    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};
        
        // Position attribute
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);
        
        // Normal attribute
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, normal);
        
        // Texture coordinate attribute
        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
        
        // Color attribute
        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[3].offset = offsetof(Vertex, color);
        
        return attributeDescriptions;
    }
};

// Primitive mesh types
enum class MeshPrimitiveType {
    CUBE,
    PLANE,
    SPHERE,
    CUSTOM
};

// Mesh class for handling 3D geometry
class Mesh {
public:
    Mesh(VulkanRenderer* renderer);
    ~Mesh();

    // Create primitive shapes
    bool CreateCube(float size = 1.0f, const glm::vec3& color = glm::vec3(1.0f));
    bool CreatePlane(float width = 1.0f, float height = 1.0f, const glm::vec3& color = glm::vec3(1.0f));
    bool CreateSphere(float radius = 1.0f, uint32_t segments = 16, const glm::vec3& color = glm::vec3(1.0f));
    
    // Create custom mesh from vertices and indices
    bool CreateFromVertices(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    
    // Draw the mesh
    void Draw(VkCommandBuffer commandBuffer);
    
    // Clean up resources
    void Cleanup();

private:
    // Create GPU buffers from mesh data
    bool CreateBuffers();

    // Mesh data
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    
    // Primitive type of this mesh
    MeshPrimitiveType m_primitiveType;
    
    // Vulkan renderer reference
    VulkanRenderer* m_renderer;
    
    // Vulkan resources
    VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory m_vertexBufferMemory = VK_NULL_HANDLE;
    VkBuffer m_indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory m_indexBufferMemory = VK_NULL_HANDLE;
};

// Singleton manager class for all meshes
class MeshManager {
public:
    static MeshManager& GetInstance() {
        static MeshManager instance;
        return instance;
    }
    
    // Delete copy and move constructors and assign operators
    MeshManager(MeshManager const&) = delete;
    MeshManager(MeshManager&&) = delete;
    MeshManager& operator=(MeshManager const&) = delete;
    MeshManager& operator=(MeshManager&&) = delete;
    
    // Initialize the manager with a renderer
    void Initialize(VulkanRenderer* renderer);
    
    // Create a primitive mesh
    std::shared_ptr<Mesh> CreateMesh(MeshPrimitiveType type);
    
    // Create a custom mesh from vertex and index data
    std::shared_ptr<Mesh> CreateCustomMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    
    // Get a mesh by ID
    std::shared_ptr<Mesh> GetMesh(const std::string& id);
    
    // Cleanup all meshes
    void Cleanup();

private:
    MeshManager() : m_renderer(nullptr), m_nextMeshId(0) {}
    ~MeshManager() = default;
    
    VulkanRenderer* m_renderer;
    std::map<std::string, std::shared_ptr<Mesh>> m_meshes;
    uint32_t m_nextMeshId;
};