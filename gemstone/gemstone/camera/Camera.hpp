#pragma once

#include <string>

#include <glm/glm.hpp>

#include "gemstone/managers/input/InputManager.hpp"
#include "gemstone/renderer/context/Context.hpp"

namespace GEM {
    class Camera;
}

class GEM::Camera {
public: // classes and enums
    struct Settings {
        float movementSpeed;
        float mouseSensitivity;
        float scrollSensitivity;
        const float maxFOVDegrees;
        const float minFOVDegrees;
        const float nearClippingPlane;
        const float farClippingPlane;

        Settings() :
            movementSpeed(2.5f),
            mouseSensitivity(0.1f),
            scrollSensitivity(1.0f),
            maxFOVDegrees(85.0f),
            minFOVDegrees(5.0f),
            nearClippingPlane(0.1f),
            farClippingPlane(100.0f)
        {}

        Settings(const Settings& other) = default;
    };

public: // public static variables
    static const std::string LOGGER_NAME;

    // To be retrieved from the application's context
    static float deltaTime;

public: // public member functions
    Camera();
    Camera(
        std::shared_ptr<GEM::Renderer::Context> p_context,
        std::shared_ptr<GEM::Managers::InputManager> p_inputManager,
        const glm::vec3 initialWorldPosition,
        const glm::vec3 initialLookVector,
        const glm::vec3 worldUpVector,
        const float initialPitch,
        const float initialYaw,
        const float initialRoll,
        const float initialFOVDegrees,
        const GEM::Camera::Settings& settings
    );
    ~Camera();
    Camera(const Camera& other) = default;

    uint32_t getID() const { return m_id; }
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void update();
    
private: // private static variables
    static uint32_t cameraCount;

private: // private member functions
    void updateOrientation();
    void updateFieldOfView();
    void updatePosition();

private: // private member variables
    const uint32_t m_id;

    const std::shared_ptr<GEM::Renderer::Context> mp_context;
    const std::shared_ptr<GEM::Managers::InputManager> mp_inputManager;

    // Orientation vectors
    glm::vec3 m_worldPosition;
    glm::vec3 m_lookVector;
    glm::vec3 m_upVector;
    glm::vec3 m_rightVector;
    const glm::vec3 m_worldUpVector; /** @todo get this from the scene that we are in */

    // Euler angles
    float m_pitch;
    float m_yaw;
    float m_roll;

    // FOV
    float m_fovDegrees;

    // Settings
    GEM::Camera::Settings m_settings;
};