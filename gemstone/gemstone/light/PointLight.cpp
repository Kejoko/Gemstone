#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "util/logger/Logger.hpp"

#include "gemstone/light/logger.hpp"
#include "gemstone/light/PositionalLight.hpp"
#include "gemstone/light/PointLight.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the ambient light class uses
 */
const std::string GEM::PointLight::LOGGER_NAME = LIGHT_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/* ------------------------------ public member functions ------------------------------ */

GEM::PointLight::PointLight(
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
    GEM::PositionalLight::PositionalLight(
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
        initialRotationAmountDegrees,
        initialDiffuseColor,
        initialSpecularColor,
        constant,
        linear,
        quadratic
    )
{
    LOG_FUNCTION_CALL_INFO("{}", nullptr);
}

GEM::PointLight::~PointLight() {
    LOG_FUNCTION_CALL_TRACE("this ptr {}", static_cast<void*>(this));
}

/* ------------------------------ private member functions ------------------------------ */

