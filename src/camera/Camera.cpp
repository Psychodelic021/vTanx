#define NOMINMAX    // Prevent Windows macro conflicts
#define GLM_ENABLE_EXPERIMENTAL // Enable experimental GLM extensions

#include "Camera.h"
#include <algorithm> // For std::clamp
#include "../../vendor/glm/gtx/euler_angles.hpp"

Camera::Camera(float aspectRatio)
    : m_aspectRatio(aspectRatio)
{
    // Initialize with default settings
    SetPosition(glm::vec3(0.0f, 1.0f, -5.0f)); // Slightly above ground, looking at origin
    SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    
    // Update matrices with initial values
    UpdateCameraVectors();
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

Camera::~Camera()
{
}

void Camera::Update(Input* input, float deltaTime)
{
    if (!input) return;

    // Handle keyboard input for camera movement based on mode
    if (m_mode == CameraMode::FirstPerson)
    {
        // First-person movement (WASD)
        if (input->IsKeyDown('W'))
            MoveForward(m_settings.movementSpeed * deltaTime);
        
        if (input->IsKeyDown('S'))
            MoveForward(-m_settings.movementSpeed * deltaTime);
        
        if (input->IsKeyDown('A'))
            MoveRight(-m_settings.movementSpeed * deltaTime);
        
        if (input->IsKeyDown('D'))
            MoveRight(m_settings.movementSpeed * deltaTime);
        
        if (input->IsKeyDown(VK_SPACE))
            MoveUp(m_settings.movementSpeed * deltaTime);
        
        if (input->IsKeyDown(VK_CONTROL))
            MoveUp(-m_settings.movementSpeed * deltaTime);
        
        // Mouse rotation for first-person
        if (input->IsMouseButtonDown(MouseButton::Right))
        {
            if (!m_isRotating)
            {
                m_isRotating = true;
                m_lastMouseX = static_cast<float>(input->GetMouseX());
                m_lastMouseY = static_cast<float>(input->GetMouseY());
            }
            else
            {
                float mouseX = static_cast<float>(input->GetMouseX());
                float mouseY = static_cast<float>(input->GetMouseY());
                
                float dx = mouseX - m_lastMouseX;
                float dy = mouseY - m_lastMouseY;
                
                Rotate(dy * m_settings.rotationSpeed, dx * m_settings.rotationSpeed);
                
                m_lastMouseX = mouseX;
                m_lastMouseY = mouseY;
            }
        }
        else
        {
            m_isRotating = false;
        }
    }
    else if (m_mode == CameraMode::ThirdPerson || m_mode == CameraMode::Orbital)
    {
        // For third-person or orbital cameras
        
        // Handle orbiting with mouse
        if (input->IsMouseButtonDown(MouseButton::Right))
        {
            if (!m_isRotating)
            {
                m_isRotating = true;
                m_lastMouseX = static_cast<float>(input->GetMouseX());
                m_lastMouseY = static_cast<float>(input->GetMouseY());
            }
            else
            {
                float mouseX = static_cast<float>(input->GetMouseX());
                float mouseY = static_cast<float>(input->GetMouseY());
                
                float dx = mouseX - m_lastMouseX;
                float dy = mouseY - m_lastMouseY;
                
                OrbitAroundTarget(dx * m_settings.rotationSpeed, dy * m_settings.rotationSpeed);
                
                m_lastMouseX = mouseX;
                m_lastMouseY = mouseY;
            }
        }
        else
        {
            m_isRotating = false;
        }
        
        // Handle zooming with mouse wheel
        ZoomTowardsTarget(input->GetMouseScrollDelta() * m_settings.zoomSpeed);
        
        if (m_mode == CameraMode::ThirdPerson)
        {
            // In third-person, we can also move the target with WASD
            glm::vec3 moveDirection(0.0f, 0.0f, 0.0f);
            
            if (input->IsKeyDown('W'))
                moveDirection.z += 1.0f;
            
            if (input->IsKeyDown('S'))
                moveDirection.z -= 1.0f;
            
            if (input->IsKeyDown('A'))
                moveDirection.x -= 1.0f;
            
            if (input->IsKeyDown('D'))
                moveDirection.x += 1.0f;
            
            // Calculate the actual movement vector by transforming the direction with the camera rotation
            if (moveDirection.x != 0.0f || moveDirection.z != 0.0f)
            {
                // Forward and right vectors projected onto the xz-plane for movement
                glm::vec3 forwardXZ = glm::vec3(m_forward.x, 0.0f, m_forward.z);
                glm::vec3 rightXZ = glm::vec3(m_right.x, 0.0f, m_right.z);
                
                // Normalize these vectors
                forwardXZ = glm::normalize(forwardXZ);
                rightXZ = glm::normalize(rightXZ);
                
                // Calculate movement vector
                glm::vec3 moveVec = forwardXZ * moveDirection.z * m_settings.movementSpeed * deltaTime +
                                   rightXZ * moveDirection.x * m_settings.movementSpeed * deltaTime;
                
                // Update target position
                m_target += moveVec;
                
                // Need to update view matrix when target changes
                UpdateViewMatrix();
            }
        }
    }
}

void Camera::SetPosition(const glm::vec3& position)
{
    m_position = position;
    UpdateViewMatrix();
}

void Camera::SetRotation(const glm::vec3& rotation)
{
    m_rotation = rotation;
    
    // Clamp pitch to avoid gimbal lock
    const float halfPi = glm::half_pi<float>();
    m_rotation.x = std::clamp(m_rotation.x, -halfPi + 0.01f, halfPi - 0.01f);
    
    UpdateCameraVectors();
    UpdateViewMatrix();
}

void Camera::SetTarget(const glm::vec3& target)
{
    m_target = target;
    
    if (m_mode == CameraMode::ThirdPerson || m_mode == CameraMode::Orbital) {
        UpdateViewMatrix();
    }
}

void Camera::SetDistance(float distance)
{
    m_distance = std::max(0.1f, distance);
    
    if (m_mode == CameraMode::ThirdPerson || m_mode == CameraMode::Orbital) {
        UpdateViewMatrix();
    }
}

void Camera::SetAspectRatio(float aspectRatio)
{
    m_aspectRatio = aspectRatio;
    UpdateProjectionMatrix();
}

void Camera::SetMode(CameraMode mode)
{
    m_mode = mode;
    UpdateViewMatrix();
}

void Camera::SetSettings(const CameraSettings& settings)
{
    m_settings = settings;
    UpdateProjectionMatrix();
}

const glm::mat4& Camera::GetViewMatrix() const
{
    return m_viewMatrix;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
    return m_projectionMatrix;
}

void Camera::MoveForward(float distance)
{
    m_position += m_forward * distance;
    UpdateViewMatrix();
}

void Camera::MoveRight(float distance)
{
    m_position += m_right * distance;
    UpdateViewMatrix();
}

void Camera::MoveUp(float distance)
{
    m_position += m_up * distance;
    UpdateViewMatrix();
}

void Camera::Rotate(float pitchDelta, float yawDelta)
{
    // Update rotation angles
    m_rotation.x += pitchDelta;
    m_rotation.y += yawDelta;
    
    // Clamp pitch to avoid gimbal lock
    const float halfPi = glm::half_pi<float>();
    m_rotation.x = std::clamp(m_rotation.x, -halfPi + 0.01f, halfPi - 0.01f);
    
    // Update camera direction vectors
    UpdateCameraVectors();
    UpdateViewMatrix();
}

void Camera::OrbitAroundTarget(float horizontalDelta, float verticalDelta)
{
    // Update rotation angles
    m_rotation.y += horizontalDelta;
    m_rotation.x -= verticalDelta;  // Invert for more natural control
    
    // Clamp pitch to avoid gimbal lock
    const float halfPi = glm::half_pi<float>();
    m_rotation.x = std::clamp(m_rotation.x, -halfPi + 0.01f, halfPi - 0.01f);
    
    // Update camera direction vectors
    UpdateCameraVectors();
    UpdateViewMatrix();
}

void Camera::ZoomTowardsTarget(float zoomDelta)
{
    // Update distance to target
    m_distance -= zoomDelta;
    m_distance = std::max(0.1f, m_distance); // Ensure minimum distance
    
    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
    if (m_mode == CameraMode::FirstPerson)
    {
        // First person view matrix - position + rotation
        glm::vec3 lookAtPoint = m_position + m_forward;
        m_viewMatrix = glm::lookAt(m_position, lookAtPoint, m_up);
    }
    else // ThirdPerson or Orbital
    {
        // Calculate camera position from target, rotation and distance
        
        // Create rotation matrix from Euler angles
        glm::mat4 rotationMatrix = glm::eulerAngleXYZ(m_rotation.x, m_rotation.y, m_rotation.z);
        
        // Calculate the offset vector (0, 0, -distance) and rotate it
        glm::vec4 offset = rotationMatrix * glm::vec4(0.0f, 0.0f, -m_distance, 0.0f);
        
        // Calculate final camera position
        m_position = m_target + glm::vec3(offset);
        
        // Look at the target
        m_viewMatrix = glm::lookAt(m_position, m_target, glm::vec3(0.0f, 1.0f, 0.0f));
    }
}

void Camera::UpdateProjectionMatrix()
{
    float fovRadians = glm::radians(m_settings.fieldOfView);
    m_projectionMatrix = glm::perspective(
        fovRadians,
        m_aspectRatio,
        m_settings.nearPlane,
        m_settings.farPlane
    );
}

void Camera::UpdateCameraVectors()
{
    // Calculate rotation matrix from euler angles
    glm::mat4 rotationMatrix = glm::eulerAngleXYZ(m_rotation.x, m_rotation.y, 0.0f);
    
    // Calculate new forward vector (default is +Z in OpenGL/Vulkan convention)
    glm::vec4 forwardVec = rotationMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    m_forward = glm::normalize(glm::vec3(forwardVec));
    
    // Get right vector by taking cross product of forward and world up
    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    m_right = glm::normalize(glm::cross(worldUp, m_forward));
    
    // Get up vector by taking cross product of forward and right
    m_up = glm::normalize(glm::cross(m_forward, m_right));
}