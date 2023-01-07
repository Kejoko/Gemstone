#include <string>
#include <vector>

#include "util/logger/Logger.hpp"

#include "gemstone/renderer/material/Material.hpp"
#include "gemstone/renderer/mesh/Mesh.hpp"
#include "gemstone/renderer/model/logger.hpp"
#include "gemstone/renderer/model/Model.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the Model class uses
 */
const std::string GEM::Renderer::Model::LOGGER_NAME = MODEL_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/**
 * @brief The count of how many models there are
 */
uint32_t GEM::Renderer::Model::modelCount = 0;

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/* ------------------------------ public member functions ------------------------------ */

GEM::Renderer::Model::Model(
    const std::string& meshFilename,
    const std::string& diffuseMapFilename,
    const std::string& specularMapFilename,
    const std::string& emissionMapFilename,
    const float shininess,
    const char* vertexShaderSource,
    const char* fragmentShaderSource
) :
    m_id(++GEM::Renderer::Model::modelCount),
    mp_mesh(std::make_shared<GEM::Renderer::Mesh>()),
    mp_material(
        std::make_shared<GEM::Renderer::Material>(
            diffuseMapFilename,
            specularMapFilename,
            emissionMapFilename,
            shininess,
            vertexShaderSource,
            fragmentShaderSource
        )
    )
{
    UNUSED(meshFilename);
    LOG_FUNCTION_CALL_INFO("id {}", m_id);
}

GEM::Renderer::Model::~Model() {
    LOG_FUNCTION_CALL_INFO("this ptr {} , id {}", static_cast<void*>(this), m_id);
}

void GEM::Renderer::Model::draw() const {
    mp_mesh->draw();
}

/* ------------------------------ private member functions ------------------------------ */
