#pragma once

#include <string>

#include <glm/glm.hpp>

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
    };

public: // public static variables
    static const std::string LOGGER_NAME;

    // To be retrieved from the application
    static float deltaTime;
    static int windowWidthPixels;
    static int windowHeightPixels;

    // All of these things are to go into the input manager
    static bool forwardsPressed;
    static bool backwardsPressed;
    static bool leftPressed;
    static bool rightPressed;
    static bool risePressed;
    static bool lowerPressed;

public: // public member functions
    Camera();
    Camera(
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

    void update();
    void updateOrientation(const float mouseXPosOffset, const float mouseYPosOffset);
    void updateFieldOfView(const float mouseYScrollOffset);
    void updatePosition();
    
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

private: // private static variables
    static uint32_t cameraCount;

private: // private member variables
    const uint32_t m_id;

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