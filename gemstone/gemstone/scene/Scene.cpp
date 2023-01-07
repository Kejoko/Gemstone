#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "util/io/FileSystem.hpp"
#include "util/logger/Logger.hpp"

#include "gemstone/object/Object.hpp"
#include "gemstone/renderer/mesh/Mesh.hpp"
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

/**
 * @brief Load the name of the scene from the scene file
 * 
 * @param filename The scene file's name
 * @return std::string The name of the scene
 */
std::string GEM::Scene::loadName(const std::string& filename) {
    LOG_FUNCTION_CALL_TRACE("filename {}", filename);

    return "Default Scene";
}

/**
 * @brief Load the camera into the scene from the scene file
 * 
 * @param p_context The context for which the camera will be drawing
 * @param p_inputManager The input manager controlling the camera
 * @param filename The name of the scene file
 * @return std::shared_ptr<GEM::Camera> The shared pointer of the camera
 */
std::shared_ptr<GEM::Camera> GEM::Scene::loadCamera(
    std::shared_ptr<GEM::Renderer::Context> p_context,
    std::shared_ptr<GEM::Managers::InputManager> p_inputManager,
    const std::string& filename
) {
    LOG_FUNCTION_CALL_TRACE(
        "context ptr {} , input manager ptr {} , filename {}",
        static_cast<void*>(p_context.get()),
        static_cast<void*>(p_inputManager.get()),
        filename
    );

    glm::vec3 cameraInitialPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraInitialLookVector = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 worldUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
    std::shared_ptr<GEM::Camera> p_camera = std::make_shared<GEM::Camera>(GEM::Camera(
        p_context,
        p_inputManager,
        cameraInitialPosition,
        cameraInitialLookVector,
        worldUpVector,
        0.0f,
        -90.0f,
        0.0f,
        60.0f,
        {}
    ));

    return p_camera;
}

GEM::Scene::AmbientLight GEM::Scene::loadAmbientLight(const std::string& filename) {
    LOG_FUNCTION_CALL_TRACE("filename {}", filename);

    return {
        glm::vec3(1.0f, 1.0f, 1.0f),
        0.2
    };
}

std::vector<std::shared_ptr<GEM::Light>> GEM::Scene::loadLights(
    const std::string& filename,
    std::shared_ptr<GEM::Renderer::ShaderProgram> p_lightShader,
    const char* lightVertexShaderSource,
    const char* lightFragmentShaderSource
) {
    UNUSED(p_lightShader);
    LOG_FUNCTION_CALL_TRACE("filename {}", filename);

    std::vector<std::shared_ptr<GEM::Light>> lightPtrs = {
        std::make_shared<GEM::Light>(
            1,
            "mesh.obj",
            "application/assets/textures/wes.png",
            "application/assets/textures/texture_coords.png",
            32.0f,
            lightVertexShaderSource,
            lightFragmentShaderSource,
            glm::vec3(1.2f, 1.0f, 4.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            0.0f,
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(1.0f, 1.0f, 1.0f)
        )
    };

    return lightPtrs;
}

/**
 * @brief Load all of the objects in the scene from the scene's file
 * 
 * @param filename The filename representing the scene
 * @param p_objectShader The default shader program to use on the objects
 * @return std::vector<std::shared_ptr<GEM::Object>> The vector containing shared pointers for all of the objects
 */
std::vector<std::shared_ptr<GEM::Object>> GEM::Scene::loadObjects(
    const std::string& filename,
    std::shared_ptr<GEM::Renderer::ShaderProgram> p_objectShader,
    const char* objectVertexShaderSource,
    const char* objectFragmentShaderSource
) {
    UNUSED(p_objectShader);
    LOG_FUNCTION_CALL_TRACE("filename {}", filename);

    // Full scene with many objects scaling and rotating
    // std::vector<std::shared_ptr<GEM::Object>> objectPtrs = {
    //     std::make_shared<GEM::Object>(0, "mesh.obj", "application/assets/textures/wes.png",                 "application/assets/textures/texture_coords.png", glm::vec3( 0.0f,  0.0f,   0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f),
    //     std::make_shared<GEM::Object>(1, "mesh.obj", "application/assets/textures/awesome_face.png",        "application/assets/textures/texture_coords.png", glm::vec3( 2.0f,  5.0f, -15.0f), glm::vec3(0.5f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f),
    //     std::make_shared<GEM::Object>(2, "mesh.obj", "application/assets/textures/brick_wall.jpg",          "application/assets/textures/texture_coords.png", glm::vec3(-1.5f, -2.2f,  -2.5f), glm::vec3(1.0f, 0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f),
    //     std::make_shared<GEM::Object>(3, "mesh.obj", "application/assets/textures/missing_texture.png",     "application/assets/textures/texture_coords.png", glm::vec3(-3.8f, -2.0f, -12.3f), glm::vec3(1.0f, 1.0f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f),
    //     std::make_shared<GEM::Object>(4, "mesh.obj", "application/assets/textures/wooden_container.jpg",    "application/assets/textures/texture_coords.png", glm::vec3( 2.4f, -0.4f,  -3.5f), glm::vec3(0.5f, 0.5f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f),
    //     std::make_shared<GEM::Object>(5, "mesh.obj", "application/assets/textures/wes.png",                 "application/assets/textures/texture_coords.png", glm::vec3(-1.7f,  3.0f,  -7.5f), glm::vec3(0.5f, 1.0f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f),
    //     std::make_shared<GEM::Object>(6, "mesh.obj", "application/assets/textures/awesome_face.png",        "application/assets/textures/texture_coords.png", glm::vec3( 1.3f, -2.0f,  -2.5f), glm::vec3(1.0f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f),
    //     std::make_shared<GEM::Object>(7, "mesh.obj", "application/assets/textures/brick_wall.jpg",          "application/assets/textures/texture_coords.png", glm::vec3( 1.5f,  2.0f,  -2.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f),
    //     std::make_shared<GEM::Object>(8, "mesh.obj", "application/assets/textures/missing_texture.png",     "application/assets/textures/texture_coords.png", glm::vec3( 1.5f,  0.2f,  -1.5f), glm::vec3(0.6f, 0.6f, 0.6f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f),
    //     std::make_shared<GEM::Object>(9, "mesh.obj", "application/assets/textures/wooden_container.jpg",    "application/assets/textures/texture_coords.png", glm::vec3(-1.3f,  1.0f,  -1.5f), glm::vec3(0.5f, 0.6f, 0.7f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f)
    // };

    std::vector<std::shared_ptr<GEM::Object>> objectPtrs = {
        std::make_shared<GEM::Object>(
            0,
            "mesh.obj",
            "application/assets/textures/wes.png",
            "application/assets/textures/texture_coords.png",
            32.0f,
            objectVertexShaderSource,
            objectFragmentShaderSource,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            0.0f
        )
    };

    return objectPtrs;
}

/* ------------------------------ public member functions ------------------------------ */

/**
 * @brief Construct a new GEM::Scene::Scene object
 * 
 * @param p_context The context for which this scene will be drawn
 * @param p_inputManager The input manager used by the players in this scene
 * @param filename The filename representing the scene. This has all of the objects and lights and such
 * @param p_lightShader The default shader program to use on the lights in the scene
 * @param p_objectShader The default shader program to use on the objects in the scene
 */
GEM::Scene::Scene(
    std::shared_ptr<GEM::Renderer::Context> p_context,
    std::shared_ptr<GEM::Managers::InputManager> p_inputManager,
    const std::string& filename,
    std::shared_ptr<GEM::Renderer::ShaderProgram> p_lightShader,
    std::shared_ptr<GEM::Renderer::ShaderProgram> p_objectShader,
    const char* lightVertexShaderSource,
    const char* lightFragmentShaderSource,
    const char* objectVertexShaderSource,
    const char* objectFragmentShaderSource
) :
    m_id(++GEM::Scene::sceneCount),
    m_filename(GEM::util::FileSystem::getFullPath(filename)),
    m_name(GEM::Scene::loadName(m_filename)),
    mp_context(p_context),
    mp_inputManager(p_inputManager),
    mp_camera(GEM::Scene::loadCamera(mp_context, mp_inputManager, m_filename)),
    m_ambientLight(GEM::Scene::loadAmbientLight(m_filename)),
    m_lightPtrs(GEM::Scene::loadLights(m_filename, p_lightShader, lightVertexShaderSource, lightFragmentShaderSource)),
    m_objectPtrs(GEM::Scene::loadObjects(m_filename, p_objectShader, objectVertexShaderSource, objectFragmentShaderSource))
{
    LOG_FUNCTION_CALL_INFO(
        "id {} , filename {} , name {} , camera id {} , object count {}",
        m_id,
        m_filename,
        m_name,
        mp_camera->getID(),
        m_objectPtrs.size()
    );
}

/**
 * @brief Destroy the GEM::Scene::Scene object
 */
GEM::Scene::~Scene() {
    LOG_FUNCTION_CALL_TRACE("this ptr {} , id {} , name {}", static_cast<void*>(this), m_id, m_name);
}

/**
 * @brief Update all of the things in the scene
 */
void GEM::Scene::update() {
    // Update the position of the camera
    mp_camera->update();

    // Update each of the objects in the scene
    for (size_t i = 0; i < m_objectPtrs.size(); ++i) {
        m_objectPtrs[i]->update();
    }
}

/* ------------------------------ private member functions ------------------------------ */

