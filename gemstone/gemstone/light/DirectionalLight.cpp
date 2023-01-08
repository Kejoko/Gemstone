#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "util/logger/Logger.hpp"

#include "gemstone/light/logger.hpp"
#include "gemstone/light/DirectionalLight.hpp"
#include "gemstone/light/Light.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the ambient light class uses
 */
const std::string GEM::DirectionalLight::LOGGER_NAME = LIGHT_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/* ------------------------------ public member functions ------------------------------ */

GEM::DirectionalLight::DirectionalLight(
    const glm::vec3& initialDiffuseColor,
    const glm::vec3& initialSpecularColor,
    const glm::vec3& initialDirection
) :
    GEM::Light::Light(
        initialDiffuseColor,
        initialSpecularColor
    ),
    m_direction(glm::normalize(initialDirection))
{
    LOG_FUNCTION_CALL_INFO("direction [ {} {} {} ]", m_direction.x, m_direction.y, m_direction.z);
}

GEM::DirectionalLight::~DirectionalLight() {
    LOG_FUNCTION_CALL_TRACE("this ptr {}", static_cast<void*>(this));
}

/* ------------------------------ private member functions ------------------------------ */

