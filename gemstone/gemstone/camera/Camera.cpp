#include "gemstone/camera/Camera.hpp"

#include <cmath>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/logger/Logger.hpp"

#include "gemstone/camera/logger.hpp"
#include "gemstone/managers/input/InputManager.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the Camera class uses
 */
const std::string GEM::Camera::LOGGER_NAME = CAMERA_LOGGER_NAME;

/**
 * @brief The counter of how many different cameras are created. This helps us determine
 * camera ids
 */
uint32_t GEM::Camera::cameraCount = 0;

/**
 * @todo get these from the application's context
 */
float GEM::Camera::deltaTime = 0;
int GEM::Camera::windowWidthPixels = 0;
int GEM::Camera::windowHeightPixels = 0;

/* ------------------------------ private static variables ------------------------------ */

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/* ------------------------------ public member functions ------------------------------ */

/**
 * @brief Construct a new GEM::Camera::Camera object from default parameters
 */
GEM::Camera::Camera() : GEM::Camera::Camera(
    nullptr,
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    0.0f,
    -90.0f,
    0.0f,
    45.0f,
    {}
) {}

/**
 * @brief Construct a new GEM::Camera::Camera object from the given parameters
 * 
 * @param p_inputManager A pointer to the input manager responsible for capturing input for this context
 * @param initialWorldPosition The initial position in the world of the camera
 * @param initialLookVector The initial look vector of the camera
 * @param worldUpVector The vecteor pointing straight up in world coordinates (usually 0, 1, 0)
 * @param initialPitch The initial pitch of the camera
 * @param initialYaw The initial yaw of the camera
 * @param initialRoll The initial roll of the camera
 * @param initialFOVDegrees The initial FOV in degrees of the camera. This is clamped by the values in the settings variable
 * @param settings The settings for the camera (clipping planes, fov min/max, movement speeds)
 */
GEM::Camera::Camera(
    std::shared_ptr<GEM::InputManager> p_inputManager,
    const glm::vec3 initialWorldPosition,
    const glm::vec3 initialLookVector,
    const glm::vec3 worldUpVector,
    const float initialPitch,
    const float initialYaw,
    const float initialRoll,
    const float initialFOVDegrees,
    const GEM::Camera::Settings& settings
) :
    m_id(++GEM::Camera::cameraCount),
    mp_inputManager(p_inputManager),
    m_worldPosition(initialWorldPosition),
    m_lookVector(initialLookVector),    // updated in the updateOrientation call
    m_upVector(),                       // updated in the updateOrientation call
    m_rightVector(),                    // updated in the updateOrientation call
    m_worldUpVector(worldUpVector),
    m_pitch(initialPitch),
    m_yaw(initialYaw),
    m_roll(initialRoll),
    m_fovDegrees(
        // Clamp the initial fov degrees to the min and max determined by settings
        initialFOVDegrees >= settings.minFOVDegrees ? 
            (
                initialFOVDegrees <= settings.maxFOVDegrees ?
                    initialFOVDegrees :
                    settings.maxFOVDegrees
            ) :
            settings.minFOVDegrees
    ),
    m_settings(settings)
{
    LOG_FUNCTION_CALL_INFO(
        "id {} , position ({} {} {}) , look ({} {} {}) , up ({} {} {}) , pitch {} , yaw {} , roll {} , fov deg {}",
        m_id,
        initialWorldPosition.x, initialWorldPosition.y, initialWorldPosition.z,
        initialLookVector.x, initialLookVector.y, initialLookVector.z,
        worldUpVector.x, worldUpVector.y, worldUpVector.z,
        initialPitch, initialYaw, initialRoll, initialFOVDegrees
    );
    UNUSED(m_roll);
    updateOrientation();
}

GEM::Camera::~Camera() {
    LOG_FUNCTION_CALL_TRACE("id {}", m_id);
}

void GEM::Camera::update() {
    updateOrientation();
    updateFieldOfView();
    updatePosition();
}

glm::mat4 GEM::Camera::getViewMatrix() {
    return glm::lookAt(m_worldPosition, m_worldPosition + m_lookVector, m_upVector);
}

glm::mat4 GEM::Camera::getProjectionMatrix() {
    return glm::perspective(
        glm::radians(m_fovDegrees), 
        static_cast<float>(GEM::Camera::windowWidthPixels) / static_cast<float>(GEM::Camera::windowHeightPixels),
        m_settings.nearClippingPlane,    
        m_settings.farClippingPlane    
    );
}

/* ------------------------------ private member functions ------------------------------ */

/**
 * @brief Update the camera's orientation (where it is looking) based on how the player has moved
 * the mouse this frame
 */
void GEM::Camera::updateOrientation() {
    // Update pitch, yaw, roll based on mouse input
    float calibratedMouseXPosOffset = mp_inputManager->getCursorXPosOffset() * m_settings.mouseSensitivity;
    float calibratedMouseYPosOffset = mp_inputManager->getCursorYPosOffset() * m_settings.mouseSensitivity;
    
    m_yaw = glm::mod(m_yaw - calibratedMouseXPosOffset, 360.0f);
    m_pitch += calibratedMouseYPosOffset;

    if (m_pitch > 89.5f) {
        m_pitch = 89.5f;
    } else if (m_pitch < -89.5f) {
        m_pitch = -89.5f;
    }

    // Update look vector based on pitch, yaw, roll
    glm::vec3 updatedLookVector;
    updatedLookVector.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    updatedLookVector.y = sin(glm::radians(m_pitch));
    updatedLookVector.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_lookVector = glm::normalize(updatedLookVector);

    // Update right and up vectors 
    m_rightVector = glm::normalize(glm::cross(m_lookVector, m_worldUpVector));
    m_upVector = glm::normalize(glm::cross(m_rightVector, m_lookVector));
}

/**
 * @brief Update the camera's field of view (zoom) based on the amount the player
 * has scrolled this frame
 */
void GEM::Camera::updateFieldOfView() {
    m_fovDegrees -= mp_inputManager->getScrollYOffset();

    if (m_fovDegrees < 5.0f) {
        m_fovDegrees = 5.0f;
    } else if (m_fovDegrees > 85.0f) {
        m_fovDegrees = 85.0f;
    }
}

/**
 * @brief Update the camera's position in the world based on the wasd, space, lshift input
 * from the player
 */
void GEM::Camera::updatePosition() {
    float calibratedMovementSpeed = m_settings.movementSpeed * GEM::Camera::deltaTime;

    if (mp_inputManager->getForwardsPressed()) {
        m_worldPosition += calibratedMovementSpeed * m_lookVector;
    }

    if (mp_inputManager->getBackwardsPressed()) {
        m_worldPosition -= calibratedMovementSpeed * m_lookVector;
    }

    if (mp_inputManager->getLeftPressed()) {
        m_worldPosition -= calibratedMovementSpeed * m_rightVector;
    }

    if (mp_inputManager->getRightPressed()) {
        m_worldPosition += calibratedMovementSpeed * m_rightVector;
    }

    if (mp_inputManager->getJumpPressed()) {
        m_worldPosition += calibratedMovementSpeed * m_worldUpVector;
    }

    if (mp_inputManager->getCrouchPressed()) {
        m_worldPosition -= calibratedMovementSpeed * m_worldUpVector;
    }
}

