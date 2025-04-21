#pragma once

#define NOMINMAX // Prevent Windows macro conflicts
#define GLM_ENABLE_EXPERIMENTAL // Enable experimental GLM extensions

#include "../../vendor/glm/glm.hpp"
#include "../../vendor/glm/gtc/matrix_transform.hpp"
#include "../input/Input.h" // Include Input.h directly

// Camera movement speed constants
struct CameraSettings {
    float movementSpeed = 5.0f;
    float rotationSpeed = 0.005f;
    float zoomSpeed = 0.2f;
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
    float fieldOfView = 45.0f; // in degrees
};

// Camera mode
enum class CameraMode {
    FirstPerson,
    ThirdPerson,
    Orbital
};

class Camera {
public:
    Camera(float aspectRatio = 16.0f / 9.0f);
    ~Camera();

    // Update camera position and orientation based on input
    void Update(Input* input, float deltaTime);

    // Set camera properties
    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& rotation); // Pitch, Yaw, Roll in radians
    void SetTarget(const glm::vec3& target);     // For third-person cameras
    void SetDistance(float distance);            // Distance from target for 3rd person
    void SetAspectRatio(float aspectRatio);
    void SetMode(CameraMode mode);
    void SetSettings(const CameraSettings& settings);

    // Get matrices
    const glm::mat4& GetViewMatrix() const;
    const glm::mat4& GetProjectionMatrix() const;

    // Get camera properties
    const glm::vec3& GetPosition() const { return m_position; }
    const glm::vec3& GetRotation() const { return m_rotation; }
    const glm::vec3& GetTarget() const { return m_target; }
    const glm::vec3& GetForward() const { return m_forward; }
    const glm::vec3& GetRight() const { return m_right; }
    const glm::vec3& GetUp() const { return m_up; }
    float GetDistance() const { return m_distance; }
    
    // For first-person camera movement
    void MoveForward(float distance);
    void MoveRight(float distance);
    void MoveUp(float distance);
    void Rotate(float pitchDelta, float yawDelta);

    // Orbit around target (for orbital and third-person cameras)
    void OrbitAroundTarget(float horizontalDelta, float verticalDelta);
    void ZoomTowardsTarget(float zoomDelta);

private:
    // Update view matrices after position/rotation changes
    void UpdateViewMatrix();
    
    // Update projection matrix after FOV/aspect ratio changes
    void UpdateProjectionMatrix();
    
    // Update camera vectors (forward, right, up) from rotation
    void UpdateCameraVectors();
    
    // Camera mode
    CameraMode m_mode = CameraMode::FirstPerson;
    
    // Camera settings
    CameraSettings m_settings;
    
    // Camera positioning
    glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f }; // Pitch, Yaw, Roll
    glm::vec3 m_target = { 0.0f, 0.0f, 0.0f };   // For third-person cameras
    float m_distance = 10.0f;                    // Distance from target
    
    // Camera vectors
    glm::vec3 m_forward = { 0.0f, 0.0f, 1.0f };
    glm::vec3 m_right = { 1.0f, 0.0f, 0.0f };
    glm::vec3 m_up = { 0.0f, 1.0f, 0.0f };
    
    // Projection parameters
    float m_aspectRatio;
    
    // Matrices
    glm::mat4 m_viewMatrix = glm::mat4(1.0f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
    
    // Input tracking
    bool m_isRotating = false;
    float m_lastMouseX = 0.0f;
    float m_lastMouseY = 0.0f;
};