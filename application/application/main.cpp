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
#include "gemstone/managers/input/logger.hpp"
#include "gemstone/managers/input/InputManager.hpp"
#include "gemstone/mesh/logger.hpp"
#include "gemstone/mesh/Mesh.hpp"
#include "gemstone/renderer/context/logger.hpp"
#include "gemstone/renderer/context/Context.hpp"
#include "gemstone/shader/logger.hpp"
#include "gemstone/shader/ShaderProgram.hpp"
#include "gemstone/texture/logger.hpp"
#include "gemstone/texture/Texture.hpp"

#include "application/core.hpp"
#include "application/shaders.hpp"

/**
 * @brief The name of the logger for the main application. A general logger
 */
#define GENERAL_LOGGER_NAME "GENERAL"
const std::string LOGGER_NAME = GENERAL_LOGGER_NAME;

/**
 * @brief Get user input
 * 
 * @param p_glfwWindow A pointer to the glfw window
 */
void processInput(GLFWwindow* p_glfwWindow) {
    std::shared_ptr<GEM::InputManager> p_inputManager = GEM::InputManager::getPtr(p_glfwWindow);

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

int main(int argc, char* argv[]) {
    UNUSED(argc);
    UNUSED(argv);

    ASSERT_GEM_VERSION();
    ASSERT_APP_VERSION();

    GEM::util::Logger::registerLoggers({
        {GENERAL_LOGGER_NAME, GEM::util::Logger::Level::trace},
        {CAMERA_LOGGER_NAME, GEM::util::Logger::Level::trace},
        {CONTEXT_LOGGER_NAME, GEM::util::Logger::Level::trace},
        {INPUT_MANAGER_LOGGER_NAME, GEM::util::Logger::Level::trace},
        {IO_LOGGER_NAME, GEM::util::Logger::Level::trace},
        {MESH_LOGGER_NAME, GEM::util::Logger::Level::trace},
        {SHADER_LOGGER_NAME, GEM::util::Logger::Level::trace},
        {TEXTURE_LOGGER_NAME, GEM::util::Logger::Level::trace}
    });

    /* ------------------------------------ initialization ------------------------------------ */

    std::shared_ptr<GEM::Context> p_context = GEM::Context::createPtr("Game boiiii", 800, 600);

    std::shared_ptr<GEM::InputManager> p_inputManager = GEM::InputManager::createPtr(p_context->getGLFWWindowPtr().get());

    glm::vec3 cameraInitialPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraInitialLookVector = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 worldUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
    GEM::Camera camera(
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
    );

    /* ------------------------------------ shader stuff ------------------------------------ */

    LOG_INFO("Creating shaders");

    std::vector<std::shared_ptr<GEM::ShaderProgram>> shaderPrograms;
    try {
        shaderPrograms.push_back(std::make_shared<GEM::ShaderProgram>(vertexShaderSource, fragmentShaderSource));
        shaderPrograms.push_back(std::make_shared<GEM::ShaderProgram>(vertexShaderSource, fragmentShader2Source));
    } catch (const std::exception& ex) {
        LOG_CRITICAL("Caught exception when trying to create shaders:\n" + std::string(ex.what()));
        return 1;
    }

    /* ------------------------------------ vertices, indices, and EBO for drawing a rectangle ------------------------------------ */

    LOG_INFO("Creating meshes");

    std::vector<std::shared_ptr<GEM::Mesh>> meshPtrs = {
        std::make_shared<GEM::Mesh>(glm::vec3( 0.0f,  0.0f,  0.0f)),
        std::make_shared<GEM::Mesh>(glm::vec3( 2.0f,  5.0f, -15.0f)),
        std::make_shared<GEM::Mesh>(glm::vec3(-1.5f, -2.2f, -2.5f)),
        std::make_shared<GEM::Mesh>(glm::vec3(-3.8f, -2.0f, -12.3f)),
        std::make_shared<GEM::Mesh>(glm::vec3( 2.4f, -0.4f, -3.5f)),
        std::make_shared<GEM::Mesh>(glm::vec3(-1.7f,  3.0f, -7.5f)),
        std::make_shared<GEM::Mesh>(glm::vec3( 1.3f, -2.0f, -2.5f)),
        std::make_shared<GEM::Mesh>(glm::vec3( 1.5f,  2.0f, -2.5f)),
        std::make_shared<GEM::Mesh>(glm::vec3( 1.5f,  0.2f, -1.5f)),
        std::make_shared<GEM::Mesh>(glm::vec3(-1.3f,  1.0f, -1.5))
    };

    /* ------------------------------------ textures ------------------------------------ */

    LOG_INFO("Creating textures");

    GEM::Texture texture(GEM::util::FileSystem::getFullPath("application/assets/textures/wes.png"), 0);
    GEM::Texture texture2(GEM::util::FileSystem::getFullPath("application/assets/textures/texture_coords.png"), 1);

    // Set the uniforms in the shader to the correct textures
    // match the value we set it to as the same value as whichever active texture it is
    shaderPrograms[0]->use();
    shaderPrograms[0]->setUniformTextureSampler("ourTexture", texture);
    shaderPrograms[0]->setUniformTextureSampler("ourTexture2", texture2);

    /* ------------------------------------ actually drawing! yay :D ------------------------------------ */

    // For frame rate
    float deltaTime = 0.0f;
    float lastFrameStartTime = 0.0f;
    float currentFrameStartTime = glfwGetTime();

    // Determine what color we want to clear the screen to
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Create the render loop
    LOG_INFO("Starting render loop");
    // while (!glfwWindowShouldClose(p_glfwWindow)) {
    while (!glfwWindowShouldClose(p_context->getGLFWWindowPtr().get())) {
        // ----- Update frame rating stuff ----- //
        currentFrameStartTime = glfwGetTime();
        deltaTime = currentFrameStartTime - lastFrameStartTime;
        GEM::Camera::deltaTime = deltaTime;
        lastFrameStartTime = currentFrameStartTime;

        // ----- Get input ----- //

        // processInput(p_glfwWindow);
        processInput(p_context->getGLFWWindowPtr().get());
        p_inputManager->collectInput();

        // ----- Rendering ----- //

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate and bind textures
        texture.activate();
        texture2.activate();

        // Set the active shader program
        shaderPrograms[0]->use();

        // Update the camera's orientation, position, and zoom
        camera.update();
        shaderPrograms[0]->setUniformMat4("viewMatrix", camera.getViewMatrix());
        shaderPrograms[0]->setUniformMat4("projectionMatrix", camera.getProjectionMatrix());

        // Render the mesh many times, each time with a different position and rotation
        for (uint32_t i = 0; i < meshPtrs.size(); ++i) {
            // Create the matrix for moving the mesh in world space
            glm::mat4 modelMatrix = glm::mat4(1.0f);

            // // Move in world space to the origin for rotation
            // modelMatrix = glm::translate(modelMatrix, -1.0f * meshPtrs[i]->getWorldPosition());

            // // Rotate an amount based on time over an axis changing over time
            // float sign = (i % 2 == 0) ? -1 : 1;
            // modelMatrix = glm::rotate(
            //     modelMatrix,
            //     static_cast<float>(glfwGetTime()) * glm::radians(sign * 45.0f) * (i+1),
            //     glm::normalize(
            //         glm::vec3(
            //             // 0.5f,
            //             // static_cast<float>(glfwGetTime()) / 10 * (i+1) * 1.0f,
            //             // i
            //             0.0f,
            //             0.0f,
            //             1.0f
            //         )
            //     )
            // );

            // // Subtract the translation part of the model matrix to put it back to the
            // // point it was at prior to spinning it
            // glm::vec4 currentTranslationColumn = glm::column(modelMatrix, 3);
            // modelMatrix = glm::translate(
            //     modelMatrix,
            //     -1.0f * glm::vec3(
            //         currentTranslationColumn.x,
            //         currentTranslationColumn.y,
            //         currentTranslationColumn.z
            //     )
            // );

            // // Move in world space to its original position
            // modelMatrix = glm::translate(modelMatrix, meshPtrs[i]->getWorldPosition());
        
            shaderPrograms[0]->setUniformMat4("modelMatrix", modelMatrix);
        
            meshPtrs[i]->draw();
        }

        // Check and call events and swap buffers
        glfwSwapBuffers(p_context->getGLFWWindowPtr().get());
    }

    GEM::InputManager::clean();
    GEM::Context::clean();
    return 0;
}