#include <string>
#include <vector>

#include "util/logger/Logger.hpp"

#include "gemstone/mesh/Mesh.hpp"
#include "gemstone/scene/logger.hpp"
#include "gemstone/scene/Scene.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the Scene class uses
 */
const std::string GEM::Scene::LOGGER_NAME = SCENE_LOGGER_NAME;

/**
 * @brief The counter of how many different scenes are created. This helps us determine
 * scene ids
 */
uint32_t GEM::Scene::sceneCount = 0;

/* ------------------------------ private static variables ------------------------------ */

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/* ------------------------------ public member functions ------------------------------ */

GEM::Scene::Scene(const std::string& name, const std::vector<GEM::Mesh>& meshes) :
    m_id(++GEM::Scene::sceneCount),
    m_name(name),
    m_meshes(meshes)
{
    UNUSED(m_id);
    UNUSED(m_name);
    UNUSED(m_meshes);
}

GEM::Scene::~Scene() {}

/* ------------------------------ private member functions ------------------------------ */

