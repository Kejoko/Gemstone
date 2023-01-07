#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "util/logger/Logger.hpp"

#include "gemstone/light/logger.hpp"
#include "gemstone/light/Light.hpp"
#include "gemstone/object/Object.hpp"
#include "gemstone/renderer/shader/ShaderProgram.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the Scene class uses
 */
const std::string GEM::Light::LOGGER_NAME = LIGHT_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/* ------------------------------ public member functions ------------------------------ */

GEM::Light::Light(
    const uint32_t id,
    const std::string& meshFilename,
    const std::string& diffuseMapFilename,
    const std::string& specularMapFilename,
    const std::string& emissionMapFilename,
    const float shininess,
    const char* vertexShaderSource,
    const char* fragmentShaderSource,
    const glm::vec3& initialWorldPosition,
    const glm::vec3& initialScale,
    const glm::vec3& initialRotationAxis,
    const float initialRotationAmountDegrees,
    const glm::vec3& initialDiffuseColor,
    const glm::vec3& initialSpecularColor
) :
    Object(
        id,
        meshFilename,
        diffuseMapFilename,
        specularMapFilename,
        emissionMapFilename,
        shininess,
        vertexShaderSource,
        fragmentShaderSource,
        initialWorldPosition,
        initialScale,
        initialRotationAxis,
        initialRotationAmountDegrees
    ),
    m_diffuseColor(initialDiffuseColor),
    m_specularColor(initialSpecularColor)
{
    LOG_FUNCTION_CALL_INFO(
        "initial diffuse color [ {} {} {} ] , initial specular color [ {} {} {} ]",
        m_diffuseColor.r, m_diffuseColor.g, m_diffuseColor.b,
        m_specularColor.r, m_specularColor.g, m_specularColor.b
    );
}

GEM::Light::~Light() {
    LOG_FUNCTION_CALL_TRACE("this ptr {}", static_cast<void*>(this));
}

/* ------------------------------ private member functions ------------------------------ */

