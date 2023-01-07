#include <memory>
#include <string>
#include <vector>

#include "util/io/FileSystem.hpp"
#include "util/logger/Logger.hpp"

#include "gemstone/renderer/material/logger.hpp"
#include "gemstone/renderer/material/Material.hpp"
#include "gemstone/renderer/shader/ShaderProgram.hpp"
#include "gemstone/renderer/texture/Texture.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the Material class uses
 */
const std::string GEM::Renderer::Material::LOGGER_NAME = MATERIAL_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/**
 * @brief The count of how many materials there are
 */
uint32_t GEM::Renderer::Material::materialCount = 0;

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/* ------------------------------ public member functions ------------------------------ */

GEM::Renderer::Material::Material(
    const std::string& diffuseMapFilename,
    const std::string& specularMapFilename,
    const std::string& emissionMapFilename,
    const float shininess,
    const char* vertexShaderSource,
    const char* fragmentShaderSource
) :
    m_id(++GEM::Renderer::Material::materialCount),
    mp_diffuseMap(std::make_shared<GEM::Renderer::Texture>(GEM::util::FileSystem::getFullPath(diffuseMapFilename), 0)),
    mp_specularMap(std::make_shared<GEM::Renderer::Texture>(GEM::util::FileSystem::getFullPath(specularMapFilename), 1)),
    mp_emissionMap(std::make_shared<GEM::Renderer::Texture>(GEM::util::FileSystem::getFullPath(emissionMapFilename), 2)),
    m_shininess(shininess),
    mp_shaderProgram(std::make_shared<GEM::Renderer::ShaderProgram>(vertexShaderSource, fragmentShaderSource))
{
    LOG_FUNCTION_CALL_INFO("id {}", m_id);
}

GEM::Renderer::Material::~Material() {
    LOG_FUNCTION_CALL_INFO("this ptr {} , id {}", static_cast<void*>(this), m_id);
}

/* ------------------------------ private member functions ------------------------------ */
