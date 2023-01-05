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
    const std::string& textureFilename,
    const std::string& textureFilename2,
    std::shared_ptr<GEM::Renderer::ShaderProgram> p_shaderProgram,
    const glm::vec3& initialWorldPosition,
    const glm::vec3& initialScale,
    const glm::vec3& initialRotationAxis,
    const float initialRotationAmountDegrees,
    const glm::vec3& initialColor
) :
    Object(
        id,
        meshFilename,
        textureFilename,
        textureFilename2,
        p_shaderProgram,
        initialWorldPosition,
        initialScale,
        initialRotationAxis,
        initialRotationAmountDegrees
    ),
    m_color(initialColor)
{
    LOG_FUNCTION_CALL_INFO("initial color [ {} {} {} ]", m_color.r, m_color.g, m_color.b);
}

GEM::Light::~Light() {
    LOG_FUNCTION_CALL_TRACE("this ptr {}", static_cast<void*>(this));
}

/* ------------------------------ private member functions ------------------------------ */

