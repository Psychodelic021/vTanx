#define NOMINMAX
#define GLM_ENABLE_EXPERIMENTAL

#include "Mesh.h"
#include "../renderer/VulkanRenderer.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include "../../vendor/glm/gtx/euler_angles.hpp"
#include "../../vendor/glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Mesh class implementation
Mesh::Mesh(VulkanRenderer* renderer) : m_renderer(renderer), m_primitiveType(MeshPrimitiveType::CUSTOM) {
    if (!renderer) {
        throw std::runtime_error("Renderer is null in Mesh constructor");
    }
}

Mesh::~Mesh() {
    Cleanup();
}

bool Mesh::CreateCube(float size, const glm::vec3& color) {
    m_primitiveType = MeshPrimitiveType::CUBE;
    
    // Half size
    float hs = size * 0.5f;
    
    // Define vertices (position, normal, texcoord, color)
    m_vertices = {
        // Front face
        { {-hs, -hs,  hs}, { 0.0f,  0.0f,  1.0f}, {0.0f, 0.0f}, color },
        { { hs, -hs,  hs}, { 0.0f,  0.0f,  1.0f}, {1.0f, 0.0f}, color },
        { { hs,  hs,  hs}, { 0.0f,  0.0f,  1.0f}, {1.0f, 1.0f}, color },
        { {-hs,  hs,  hs}, { 0.0f,  0.0f,  1.0f}, {0.0f, 1.0f}, color },
        
        // Back face
        { { hs, -hs, -hs}, { 0.0f,  0.0f, -1.0f}, {0.0f, 0.0f}, color },
        { {-hs, -hs, -hs}, { 0.0f,  0.0f, -1.0f}, {1.0f, 0.0f}, color },
        { {-hs,  hs, -hs}, { 0.0f,  0.0f, -1.0f}, {1.0f, 1.0f}, color },
        { { hs,  hs, -hs}, { 0.0f,  0.0f, -1.0f}, {0.0f, 1.0f}, color },
        
        // Top face
        { {-hs,  hs,  hs}, { 0.0f,  1.0f,  0.0f}, {0.0f, 0.0f}, color },
        { { hs,  hs,  hs}, { 0.0f,  1.0f,  0.0f}, {1.0f, 0.0f}, color },
        { { hs,  hs, -hs}, { 0.0f,  1.0f,  0.0f}, {1.0f, 1.0f}, color },
        { {-hs,  hs, -hs}, { 0.0f,  1.0f,  0.0f}, {0.0f, 1.0f}, color },
        
        // Bottom face
        { {-hs, -hs, -hs}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}, color },
        { { hs, -hs, -hs}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}, color },
        { { hs, -hs,  hs}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}, color },
        { {-hs, -hs,  hs}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}, color },
        
        // Right face
        { { hs, -hs,  hs}, { 1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, color },
        { { hs, -hs, -hs}, { 1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}, color },
        { { hs,  hs, -hs}, { 1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, color },
        { { hs,  hs,  hs}, { 1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}, color },
        
        // Left face
        { {-hs, -hs, -hs}, {-1.0f,  0.0f,  0.0f}, {0.0f, 0.0f}, color },
        { {-hs, -hs,  hs}, {-1.0f,  0.0f,  0.0f}, {1.0f, 0.0f}, color },
        { {-hs,  hs,  hs}, {-1.0f,  0.0f,  0.0f}, {1.0f, 1.0f}, color },
        { {-hs,  hs, -hs}, {-1.0f,  0.0f,  0.0f}, {0.0f, 1.0f}, color }
    };
    
    // Define indices for triangles
    m_indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        
        // Back face
        4, 5, 6, 6, 7, 4,
        
        // Top face
        8, 9, 10, 10, 11, 8,
        
        // Bottom face
        12, 13, 14, 14, 15, 12,
        
        // Right face
        16, 17, 18, 18, 19, 16,
        
        // Left face
        20, 21, 22, 22, 23, 20
    };
    
    return CreateBuffers();
}

bool Mesh::CreatePlane(float width, float height, const glm::vec3& color) {
    m_primitiveType = MeshPrimitiveType::PLANE;
    
    // Half width and height
    float hw = width * 0.5f;
    float hh = height * 0.5f;
    
    // Define vertices (position, normal, texcoord, color)
    m_vertices = {
        { {-hw, 0.0f, -hh}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}, color },
        { { hw, 0.0f, -hh}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, color },
        { { hw, 0.0f,  hh}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, color },
        { {-hw, 0.0f,  hh}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, color }
    };
    
    // Define indices for triangles
    m_indices = {
        0, 1, 2, 2, 3, 0
    };
    
    return CreateBuffers();
}

bool Mesh::CreateSphere(float radius, uint32_t segments, const glm::vec3& color) {
    m_primitiveType = MeshPrimitiveType::SPHERE;
    
    // Clear previous data
    m_vertices.clear();
    m_indices.clear();
    
    // Create sphere using stacks and slices
    uint32_t stacks = segments;
    uint32_t slices = segments * 2;
    
    // Vertices
    // Add top vertex
    m_vertices.push_back({ {0.0f, radius, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.5f, 0.0f}, color });
    
    // Add middle vertices
    for (uint32_t i = 0; i < stacks - 1; i++) {
        float phi = glm::pi<float>() * (i + 1) / stacks;
        float y = radius * cos(phi);
        float r = radius * sin(phi);
        
        for (uint32_t j = 0; j < slices; j++) {
            float theta = 2.0f * glm::pi<float>() * j / slices;
            float x = r * sin(theta);
            float z = r * cos(theta);
            
            // Normalized position is the normal for a sphere
            glm::vec3 position(x, y, z);
            glm::vec3 normal = glm::normalize(position);
            
            // Texture coordinates
            float u = static_cast<float>(j) / slices;
            float v = static_cast<float>(i + 1) / stacks;
            
            m_vertices.push_back({ position, normal, {u, v}, color });
        }
    }
    
    // Add bottom vertex
    m_vertices.push_back({ {0.0f, -radius, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.5f, 1.0f}, color });
    
    // Indices
    // Top cap
    for (uint32_t j = 0; j < slices; j++) {
        m_indices.push_back(0);
        m_indices.push_back(1 + j);
        m_indices.push_back(1 + ((j + 1) % slices));
    }
    
    // Middle sections
    for (uint32_t i = 0; i < stacks - 2; i++) {
        uint32_t baseIndex = 1 + i * slices;
        uint32_t nextBaseIndex = baseIndex + slices;
        
        for (uint32_t j = 0; j < slices; j++) {
            uint32_t currentIndex = baseIndex + j;
            uint32_t nextIndex = baseIndex + ((j + 1) % slices);
            uint32_t nextRowIndex = nextBaseIndex + j;
            uint32_t nextRowNextIndex = nextBaseIndex + ((j + 1) % slices);
            
            // Two triangles per quad
            m_indices.push_back(currentIndex);
            m_indices.push_back(nextRowIndex);
            m_indices.push_back(nextIndex);
            
            m_indices.push_back(nextIndex);
            m_indices.push_back(nextRowIndex);
            m_indices.push_back(nextRowNextIndex);
        }
    }
    
    // Bottom cap
    uint32_t bottomIndex = static_cast<uint32_t>(m_vertices.size() - 1);
    uint32_t baseIndex = bottomIndex - slices;
    for (uint32_t j = 0; j < slices; j++) {
        m_indices.push_back(bottomIndex);
        m_indices.push_back(baseIndex + ((j + 1) % slices));
        m_indices.push_back(baseIndex + j);
    }
    
    return CreateBuffers();
}

bool Mesh::CreateFromVertices(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    m_primitiveType = MeshPrimitiveType::CUSTOM;
    m_vertices = vertices;
    m_indices = indices;
    
    return CreateBuffers();
}

bool Mesh::CreateBuffers() {
    if (m_vertices.empty()) {
        return false;
    }
    
    VkDeviceSize vertexBufferSize = sizeof(m_vertices[0]) * m_vertices.size();
    
    // Create vertex buffer
    BufferCreateInfo vertexBufferInfo = {};
    vertexBufferInfo.size = vertexBufferSize;
    vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    vertexBufferInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    
    if (!m_renderer->CreateBuffer(vertexBufferInfo, m_vertexBuffer, m_vertexBufferMemory)) {
        return false;
    }
    
    // Copy vertex data to buffer
    void* data;
    vkMapMemory(m_renderer->GetDevice(), m_vertexBufferMemory, 0, vertexBufferSize, 0, &data);
    memcpy(data, m_vertices.data(), (size_t)vertexBufferSize);
    vkUnmapMemory(m_renderer->GetDevice(), m_vertexBufferMemory);
    
    // Create index buffer if indices exist
    if (!m_indices.empty()) {
        VkDeviceSize indexBufferSize = sizeof(m_indices[0]) * m_indices.size();
        
        BufferCreateInfo indexBufferInfo = {};
        indexBufferInfo.size = indexBufferSize;
        indexBufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        indexBufferInfo.properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
        
        if (!m_renderer->CreateBuffer(indexBufferInfo, m_indexBuffer, m_indexBufferMemory)) {
            return false;
        }
        
        // Copy index data to buffer
        void* indexData;
        vkMapMemory(m_renderer->GetDevice(), m_indexBufferMemory, 0, indexBufferSize, 0, &indexData);
        memcpy(indexData, m_indices.data(), (size_t)indexBufferSize);
        vkUnmapMemory(m_renderer->GetDevice(), m_indexBufferMemory);
    }
    
    return true;
}

void Mesh::Draw(VkCommandBuffer commandBuffer) {
    if (m_vertexBuffer == VK_NULL_HANDLE) {
        return;
    }
    
    VkBuffer vertexBuffers[] = {m_vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    
    if (m_indexBuffer != VK_NULL_HANDLE) {
        vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_indices.size()), 1, 0, 0, 0);
    } else {
        vkCmdDraw(commandBuffer, static_cast<uint32_t>(m_vertices.size()), 1, 0, 0);
    }
}

void Mesh::Cleanup() {
    if (m_renderer) {
        VkDevice device = m_renderer->GetDevice();
        
        if (m_indexBuffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(device, m_indexBuffer, nullptr);
            m_indexBuffer = VK_NULL_HANDLE;
        }
        
        if (m_indexBufferMemory != VK_NULL_HANDLE) {
            vkFreeMemory(device, m_indexBufferMemory, nullptr);
            m_indexBufferMemory = VK_NULL_HANDLE;
        }
        
        if (m_vertexBuffer != VK_NULL_HANDLE) {
            vkDestroyBuffer(device, m_vertexBuffer, nullptr);
            m_vertexBuffer = VK_NULL_HANDLE;
        }
        
        if (m_vertexBufferMemory != VK_NULL_HANDLE) {
            vkFreeMemory(device, m_vertexBufferMemory, nullptr);
            m_vertexBufferMemory = VK_NULL_HANDLE;
        }
    }
}

// MeshManager implementation
void MeshManager::Initialize(VulkanRenderer* renderer) {
    m_renderer = renderer;
    m_nextMeshId = 0;
}

std::shared_ptr<Mesh> MeshManager::CreateMesh(MeshPrimitiveType type) {
    auto mesh = std::make_shared<Mesh>(m_renderer);
    
    bool created = false;
    switch (type) {
        case MeshPrimitiveType::CUBE:
            created = mesh->CreateCube();
            break;
        case MeshPrimitiveType::PLANE:
            created = mesh->CreatePlane();
            break;
        case MeshPrimitiveType::SPHERE:
            created = mesh->CreateSphere();
            break;
        default:
            return nullptr;
    }
    
    if (!created) {
        return nullptr;
    }
    
    // Generate a unique ID for the mesh
    std::string id = "mesh_" + std::to_string(m_nextMeshId++);
    m_meshes[id] = mesh;
    
    return mesh;
}

std::shared_ptr<Mesh> MeshManager::CreateCustomMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    auto mesh = std::make_shared<Mesh>(m_renderer);
    
    if (!mesh->CreateFromVertices(vertices, indices)) {
        return nullptr;
    }
    
    // Generate a unique ID for the mesh
    std::string id = "mesh_" + std::to_string(m_nextMeshId++);
    m_meshes[id] = mesh;
    
    return mesh;
}

std::shared_ptr<Mesh> MeshManager::GetMesh(const std::string& id) {
    auto it = m_meshes.find(id);
    if (it != m_meshes.end()) {
        return it->second;
    }
    
    return nullptr;
}

void MeshManager::Cleanup() {
    m_meshes.clear();
}