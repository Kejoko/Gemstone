#include <cmath>
#include <memory>
#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/io/FileSystem.hpp"
#include "util/logger/Logger.hpp"

#include "gemstone/object/logger.hpp"
#include "gemstone/object/Object.hpp"
#include "gemstone/renderer/model/Model.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the Scene class uses
 */
const std::string GEM::Object::LOGGER_NAME = OBJECT_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/**
 * @brief Load the mesh from the desired file
 * 
 * @param meshFilename The full path to the file containing the mesh
 * @return std::shared_ptr<GEM::Renderer::Mesh> The shared pointer to the mesh contained within the file
 */
std::shared_ptr<GEM::Renderer::Mesh> GEM::Object::loadMesh(const std::string& meshFilename) {
    LOG_FUNCTION_CALL_TRACE("mesh filename {}", meshFilename);
    return std::make_shared<GEM::Renderer::Mesh>();
}

/**
 * @brief Load a texture at the specified file
 * 
 * @param textureFilename The full path to the texture file
 * @return std::shared_ptr<GEM::Renderer::Texture> The shared pointer containing the texture
 */
std::shared_ptr<GEM::Renderer::Texture> GEM::Object::loadTexture(const std::string& textureFilename, const uint32_t index) {
    LOG_FUNCTION_CALL_TRACE("texture filename {} , index {}", textureFilename, index);
    return std::make_shared<GEM::Renderer::Texture>(textureFilename, index);
}

/* ------------------------------ public member functions ------------------------------ */

/**
 * @brief Construct a new GEM::Object::Object object
 * 
 * @param id The id of this object in the scene
 * @param meshFilename The file where the object's mesh is stored
 * @param textureFilename The file where the object's texture is stored
 * @param textureFilename2 The file where the object's texture is stored
 * @param p_shaderProgram The shader program for this object to use
 * @param initialWorldPosition The initial position in the world the object will spawn in
 * @param initialScale The initial scale the object has
 * @param initialRotationAxis The initial axis of rotation the object has
 * @param initialRotationAmountDegrees The initial amount of rotation in degrees that the
 *  object has about its rotation axis
 */
GEM::Object::Object(
    const uint32_t id,
    const std::string& meshFilename,
    const std::string& textureFilename,
    const std::string& textureFilename2,
    const float shininess,
    const char* vertexShaderSource,
    const char* fragmentShaderSource,
    const glm::vec3& initialWorldPosition,
    const glm::vec3& initialScale,
    const glm::vec3& initialRotationAxis,
    const float initialRotationAmountDegrees
) :
    m_id(id),
    mp_model(
        std::make_shared<GEM::Renderer::Model>(
            meshFilename,
            textureFilename,
            textureFilename2,
            shininess,
            vertexShaderSource,
            fragmentShaderSource
        )
    ),
    m_worldPosition(initialWorldPosition),
    m_scale(initialScale),
    m_rotationAxis(initialRotationAxis),
    m_rotationAmountDegrees(initialRotationAmountDegrees)
{
    LOG_FUNCTION_ENTRY_INFO(
        "id {} , initial world position [ {} {} {} ]",
        m_id,
        m_worldPosition.x, m_worldPosition.y, m_worldPosition.z
    );
}

/**
 * @brief Destroy the GEM::Object::Object object
 */
GEM::Object::~Object() {
    LOG_FUNCTION_CALL_TRACE("this ptr {} , id {}", static_cast<void*>(this), m_id);
}

/**
 * @brief Update the object within the world (position, scale, rotation, etc)
 */
void GEM::Object::update() {
    // const float glfwTime = glfwGetTime();

    // m_rotationAxis = glm::vec3(
    //     std::sin(glfwTime),
    //     std::sin(static_cast<float>(glfwTime) / 10 * (m_id + 1) * 1.0f),
    //     std::cos(glfwTime)
    // );

    // m_rotationAmountDegrees = static_cast<float>(m_id + 0.65 * glfwTime);

    // glm::vec3 scaleOffset = 0.0015f * glm::vec3(
    //     std::sin(1 * m_id + glfwTime),
    //     std::sin(2 * m_id + glfwTime),
    //     std::sin(3 * m_id + glfwTime)
    // );
    // m_scale = m_scale + scaleOffset;
}

/**
 * @brief Draw the mesh
 */
void GEM::Object::draw() const {
    mp_model->getMeshPtr()->draw();
}

/**
 * @brief Calculate the model matrix using the objects world position, its local rotation, and its scale
 * 
 * @note Order matters because matrix multiplication is not commutative so we must scale first, then
 * rotate, then translate
 * 1. scale
 * 2. rotate
 * 3. translate
 * 
 * @return glm::mat4 The objects model matrix
 */
glm::mat4 GEM::Object::getModelMatrix() const {
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Translate to world position
    modelMatrix = glm::translate(
        modelMatrix,
        m_worldPosition
    );

    // Rotate to proper local rotation
    modelMatrix = glm::rotate(
        modelMatrix,
        m_rotationAmountDegrees,
        glm::normalize(m_rotationAxis)
    );

    // Scale to correct size
    modelMatrix = glm::scale(
        modelMatrix,
        m_scale
    );

    return modelMatrix;
}

/* ------------------------------ private member functions ------------------------------ */

