#include <cmath>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/macros.hpp"
#include "util/platform.hpp"
#include "util/io/logger.hpp"
#include "util/io/FileSystem.hpp"
#include "util/logger/Logger.hpp"

#include "gemstone/core.hpp"
#include "gemstone/camera/logger.hpp"
#include "gemstone/camera/Camera.hpp"
#include "gemstone/light/logger.hpp"
#include "gemstone/light/Light.hpp"
#include "gemstone/object/logger.hpp"
#include "gemstone/object/Object.hpp"
#include "gemstone/scene/logger.hpp"
#include "gemstone/scene/Scene.hpp"
#include "gemstone/managers/input/logger.hpp"
#include "gemstone/managers/input/InputManager.hpp"
#include "gemstone/renderer/context/logger.hpp"
#include "gemstone/renderer/context/Context.hpp"
#include "gemstone/renderer/mesh/logger.hpp"
#include "gemstone/renderer/mesh/Mesh.hpp"
#include "gemstone/renderer/shader/logger.hpp"
#include "gemstone/renderer/shader/ShaderProgram.hpp"
#include "gemstone/renderer/texture/logger.hpp"
#include "gemstone/renderer/texture/Texture.hpp"

#include "application/core.hpp"
#include "application/shaders.hpp"

/**
 * @brief The name of the logger for the main application. A general logger
 */
#define GENERAL_LOGGER_NAME "GENERAL"
const std::string LOGGER_NAME = GENERAL_LOGGER_NAME;

#define SHADER1_INDEX 0
#define SHADER2_INDEX 1
#define OBJECT_LIGHTING_INDEX 2
#define LIGHT_LIGHTING_INDEX 3

#define CURRENT_OBJECT_SHADER_INDEX OBJECT_LIGHTING_INDEX
#define CURRENT_LIGHT_SHADER_INDEX LIGHT_LIGHTING_INDEX

void processInput(GLFWwindow* p_glfwWindow);

void render(
    const std::shared_ptr<const GEM::Camera> p_camera,
    const GEM::Scene::AmbientLight& ambientLight,
    const std::vector<std::shared_ptr<GEM::Object>>& objectPtrs,
    const std::vector<std::shared_ptr<GEM::Light>>& lightPtrs
);

int main(int argc, char* argv[]) {
    UNUSED(argc);
    UNUSED(argv);

    ASSERT_GEM_VERSION();
    ASSERT_APP_VERSION();

    GEM::util::Logger::registerLoggers({
        {GENERAL_LOGGER_NAME, GEM::util::Logger::Level::error},
        {CAMERA_LOGGER_NAME, GEM::util::Logger::Level::error},
        {CONTEXT_LOGGER_NAME, GEM::util::Logger::Level::error},
        {INPUT_MANAGER_LOGGER_NAME, GEM::util::Logger::Level::error},
        {IO_LOGGER_NAME, GEM::util::Logger::Level::error},
        {LIGHT_LOGGER_NAME, GEM::util::Logger::Level::trace},
        {MESH_LOGGER_NAME, GEM::util::Logger::Level::error},
        {OBJECT_LOGGER_NAME, GEM::util::Logger::Level::trace},
        {SCENE_LOGGER_NAME, GEM::util::Logger::Level::error},
        {SHADER_LOGGER_NAME, GEM::util::Logger::Level::error},
        {TEXTURE_LOGGER_NAME, GEM::util::Logger::Level::error}
    });

    /* ------------------------------------ initialization ------------------------------------ */

    std::shared_ptr<GEM::Renderer::Context> p_context = GEM::Renderer::Context::createPtr("Game boiiii", 800, 600);

    std::shared_ptr<GEM::Managers::InputManager> p_inputManager = GEM::Managers::InputManager::createPtr(p_context->getGLFWWindowPtr().get());

    /* ------------------------------------ shader stuff ------------------------------------ */

    LOG_INFO("Creating shaders");

    std::vector<std::shared_ptr<GEM::Renderer::ShaderProgram>> shaderProgramPtrs;
    try {
        shaderProgramPtrs.push_back(std::make_shared<GEM::Renderer::ShaderProgram>(vertexShaderSource, fragmentShaderSource));
        shaderProgramPtrs.push_back(std::make_shared<GEM::Renderer::ShaderProgram>(vertexShaderSource, fragmentShader2Source));
        shaderProgramPtrs.push_back(std::make_shared<GEM::Renderer::ShaderProgram>(strippedVertexShaderSource, objectLightingFragShaderSource));
        shaderProgramPtrs.push_back(std::make_shared<GEM::Renderer::ShaderProgram>(strippedVertexShaderSource, lightLightingFragShaderSource));
    } catch (const std::exception& ex) {
        LOG_CRITICAL("Caught exception when trying to create shaders:\n" + std::string(ex.what()));
        return 1;
    }

    /* ------------------------------------ create the scene ------------------------------------ */

    std::shared_ptr<GEM::Scene> p_scene = std::make_shared<GEM::Scene>(
        p_context,
        p_inputManager,
        "some_scene_file.json",
        shaderProgramPtrs[CURRENT_LIGHT_SHADER_INDEX],
        shaderProgramPtrs[CURRENT_OBJECT_SHADER_INDEX]
    );

    /* ------------------------------------ actually drawing! yay :D ------------------------------------ */

    // For frame rate
    float deltaTime = 0.0f;
    float lastFrameStartTime = 0.0f;
    float currentFrameStartTime = glfwGetTime();

    // Determine what color we want to clear the screen to
    glClearColor(0.125f, 0.125f, 0.125f, 1.0f);

    // Create the render loop
    LOG_INFO("Starting render loop");
    while (!glfwWindowShouldClose(p_context->getGLFWWindowPtr().get())) {
        
        // ----- Update frame rating stuff ----- //

        currentFrameStartTime = glfwGetTime();
        deltaTime = currentFrameStartTime - lastFrameStartTime;
        GEM::Camera::deltaTime = deltaTime;
        lastFrameStartTime = currentFrameStartTime;

        // ----- Get input and update the scene ----- //

        processInput(p_context->getGLFWWindowPtr().get());
        p_inputManager->collectInput();

        p_scene->update();

        // ----- Rendering ----- //
        
        render(p_scene->getCameraPtr(), p_scene->getAmbientLight(), p_scene->getObjectPtrs(), p_scene->getLightPtrs());

        // ----- Check and call events and swap buffers before next pass ----- //

        glfwSwapBuffers(p_context->getGLFWWindowPtr().get());
    }

    GEM::Managers::InputManager::clean();
    GEM::Renderer::Context::clean();
    return 0;
}

void processInput(GLFWwindow* p_glfwWindow) {
    std::shared_ptr<GEM::Managers::InputManager> p_inputManager = GEM::Managers::InputManager::getPtr(p_glfwWindow);

    // Put us into wireframe mode if we hit the '1' key
    if (p_inputManager->getPolygonWireframePressed()) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // Put us into fill mode if we hit the '2' key
    if (p_inputManager->getPolygonFillPressed()) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Bring the cursor back if we hit the escape key
    if (p_inputManager->getPausePressed()) {
        glfwSetInputMode(p_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    // End it all! ... if the user presses the quit key ...
    if (p_inputManager->getQuitPressed()) {
        glfwSetWindowShouldClose(p_glfwWindow, true);
    }
}

void render(
    const std::shared_ptr<const GEM::Camera> p_camera,
    const GEM::Scene::AmbientLight& ambientLight,
    const std::vector<std::shared_ptr<GEM::Object>>& objectPtrs,
    const std::vector<std::shared_ptr<GEM::Light>>& lightPtrs
) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 lightColor(0.0f, 0.0f, 0.0f);
    glm::vec4 lightPosition(0.0f, 0.0f, 0.0f, 0.0f);

    // Render each of the lights
    for (uint32_t i = 0; i < lightPtrs.size(); ++i) {
        // Set the active shader program
        lightPtrs[i]->getShaderProgramPtr()->use();
        lightPtrs[i]->getShaderProgramPtr()->setUniformVec3("lightColor", lightPtrs[i]->getColor());

        // Set the uniform matrices for where the camera is oriented
        lightPtrs[i]->getShaderProgramPtr()->setUniformMat4("viewMatrix", p_camera->getViewMatrix());
        lightPtrs[i]->getShaderProgramPtr()->setUniformMat4("projectionMatrix", p_camera->getProjectionMatrix());

        // Create the matrix for moving the mesh in world space and assign it to the shader
        lightPtrs[i]->getShaderProgramPtr()->setUniformMat4("modelMatrix", lightPtrs[i]->getModelMatrix());
    
        // Draw the object
        lightPtrs[i]->draw();

        lightColor = lightPtrs[i]->getColor();
        lightPosition = {lightPtrs[i]->getWorldPosition(), 1.0f};
    }

    // Render each of the meshes
    for (uint32_t i = 0; i < objectPtrs.size(); ++i) {
        // Set the active shader program
        objectPtrs[i]->getShaderProgramPtr()->use();
        objectPtrs[i]->getShaderProgramPtr()->setUniformVec3("ambientLightColor", ambientLight.color);
        objectPtrs[i]->getShaderProgramPtr()->setUniformFloat("ambientLightStrength", ambientLight.strength);
        // objectPtrs[i]->getShaderProgramPtr()->setUniformVec4("lightColor", lightColor);
        // objectPtrs[i]->getShaderProgramPtr()->setUniformVec4("lightPosition", lightPosition);
        objectPtrs[i]->getShaderProgramPtr()->setUniformVec3("objectColor", {1.0f, 0.5f, 0.31f});

        // // Activate and bind textures the current object is using then tell the shader to use them
        // objectPtrs[i]->getTexturePtr()->activate();
        // objectPtrs[i]->getTexture2Ptr()->activate();
        // objectPtrs[i]->getShaderProgramPtr()->setUniformTextureSampler("ourTexture", objectPtrs[i]->getTexturePtr());
        // objectPtrs[i]->getShaderProgramPtr()->setUniformTextureSampler("ourTexture2", objectPtrs[i]->getTexture2Ptr());

        // Set the uniform matrices for where the camera is oriented
        objectPtrs[i]->getShaderProgramPtr()->setUniformMat4("viewMatrix", p_camera->getViewMatrix());
        objectPtrs[i]->getShaderProgramPtr()->setUniformMat4("projectionMatrix", p_camera->getProjectionMatrix());

        // Create the matrix for moving the mesh in world space and assign it to the shader
        objectPtrs[i]->getShaderProgramPtr()->setUniformMat4("modelMatrix", objectPtrs[i]->getModelMatrix());
    
        // Draw the object
        objectPtrs[i]->draw();
    }
}
