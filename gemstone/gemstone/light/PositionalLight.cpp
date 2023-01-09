#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "util/logger/Logger.hpp"

#include "gemstone/light/logger.hpp"
#include "gemstone/light/Light.hpp"
#include "gemstone/light/PositionalLight.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the ambient light class uses
 */
const std::string GEM::PositionalLight::LOGGER_NAME = LIGHT_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/* ------------------------------ public member functions ------------------------------ */

GEM::PositionalLight::PositionalLight(
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
        const glm::vec3& initialSpecularColor,
        const float constant,
        const float linear,
        const float quadratic
) :
    GEM::Object::Object(
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
    GEM::Light::Light(
        initialDiffuseColor,
        initialSpecularColor
    ),
    m_constant(constant),
    m_linear(linear),
    m_quadratic(quadratic)
{
    LOG_FUNCTION_CALL_INFO("constant {} , linear {} , quadratic {}", m_constant, m_linear, m_quadratic);
}

GEM::PositionalLight::~PositionalLight() {
    LOG_FUNCTION_CALL_TRACE("this ptr {}", static_cast<void*>(this));
}

/* ------------------------------ private member functions ------------------------------ */

