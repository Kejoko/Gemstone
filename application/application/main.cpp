#include <cmath>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// #include <stb/stb_image.h>

#include "util/macros.hpp"
#include "util/platform.hpp"
#include "util/io/logger.hpp"
#include "util/io/FileSystem.hpp"
#include "util/logger/Logger.hpp"

#include "gemstone/core.hpp"
#include "gemstone/application/logger.hpp"
#include "gemstone/application/Application.hpp"
#include "gemstone/mesh/logger.hpp"
#include "gemstone/mesh/Mesh.hpp"
#include "gemstone/scene/logger.hpp"
#include "gemstone/scene/Scene.hpp"
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
 * @brief A function that gets called every tim e the window is resized
 * 
 * @param p_glfwWindow A pointer to the glfw window
 * @param updatedWindowWidthPixels The updated width of the window in pixels
 * @param updatedWindowHeightPixels The updated height of the window in pixels
 */
void framebufferSizeCallback(GLFWwindow* p_glfwWindow, int updatedWindowWidthPixels, int updatedWindowHeightPixels) {
    UNUSED(p_glfwWindow);
    glViewport(0, 0, updatedWindowWidthPixels, updatedWindowHeightPixels);
}

/**
 * @brief Get user input
 * 
 * @param p_glfwWindow A pointer to the glfw window
 */
void processInput(GLFWwindow* p_glfwWindow) {
    // Put us into wireframe mode if we hit the '1' key
    if (glfwGetKey(p_glfwWindow, GLFW_KEY_1) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // Put us into fill mode if we hit the '2' key
    if (glfwGetKey(p_glfwWindow, GLFW_KEY_2) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // End it all! ... if the user presses the quit key ...
    if (glfwGetKey(p_glfwWindow, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(p_glfwWindow, true);
    }
}

glm::vec3 processCameraPosition(
    GLFWwindow* p_glfwWindow,
    const float deltaTime,
    const glm::vec3& currentCameraPosition,
    const glm::vec3& currentCameraLookVector,
    const glm::vec3& currentCameraUpVector,
    const glm::vec3& worldUpVector
) {
    const float cameraSpeed = 2.5f * deltaTime;
    glm::vec3 currentCameraRightVector = glm::normalize(glm::cross(currentCameraLookVector, currentCameraUpVector));
    glm::vec3 updatedCameraPosition = currentCameraPosition;

    if (glfwGetKey(p_glfwWindow, GLFW_KEY_W) == GLFW_PRESS) {
        updatedCameraPosition += cameraSpeed * currentCameraLookVector;
    }

    if (glfwGetKey(p_glfwWindow, GLFW_KEY_S) == GLFW_PRESS) {
        updatedCameraPosition -= cameraSpeed * currentCameraLookVector;
    }

    if (glfwGetKey(p_glfwWindow, GLFW_KEY_A) == GLFW_PRESS) {
        updatedCameraPosition -= cameraSpeed * currentCameraRightVector;
    }

    if (glfwGetKey(p_glfwWindow, GLFW_KEY_D) == GLFW_PRESS) {
        updatedCameraPosition += cameraSpeed * currentCameraRightVector;
    }

    if (glfwGetKey(p_glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
        updatedCameraPosition += cameraSpeed * worldUpVector;
    }

    if (glfwGetKey(p_glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        updatedCameraPosition -= cameraSpeed * worldUpVector;
    }

    return updatedCameraPosition;
}

float pitch = 0.0f;
float yaw = -90.0f;
float lastMouseXPos = 0;
float lastMouseYPos = 0;
bool mousePositionHasUpdated = false;
void mouseInputCallback(GLFWwindow* p_glfwWindow, double mouseXPos, double mouseYPos) {
    UNUSED(p_glfwWindow);

    if (!mousePositionHasUpdated) {
        lastMouseXPos = mouseXPos;
        lastMouseYPos = mouseYPos;
        mousePositionHasUpdated = true;
    }

    float xPosOffset = mouseXPos - lastMouseXPos;
    float yPosOffset = mouseYPos - lastMouseYPos;
    lastMouseXPos = mouseXPos;
    lastMouseYPos = mouseYPos;

    const float sensitivity = 0.1f;
    xPosOffset *= sensitivity;
    yPosOffset *= sensitivity;

    yaw = glm::mod(yaw + xPosOffset, 360.0f);
    pitch -= yPosOffset;

    // Clamp pitch to not look more than straight up or straight down
    if (pitch > 89.5f) {
        pitch = 89.5f;
    } else if (pitch < -89.5f) {
        pitch = -89.5f;
    }
}

float fovDegrees = 60.0f;
void mouseZoomCallback(GLFWwindow* p_glfwWindow, double xScrollOffset, double yScrollOffset) {
    UNUSED(p_glfwWindow);
    UNUSED(xScrollOffset);

    fovDegrees -= (float)yScrollOffset;
    if (fovDegrees < 5.0f) {
        fovDegrees = 5.0f;
    } else if (fovDegrees > 85.0f) {
        fovDegrees = 85.0f;
    }
}

int main(int argc, char* argv[]) {
    UNUSED(argc);
    UNUSED(argv);

    ASSERT_GEM_VERSION();
    ASSERT_APP_VERSION();

    GEM::util::Logger::registerLoggers({
        {GENERAL_LOGGER_NAME, GEM::util::Logger::Level::info},
        {APPLICATION_LOGGER_NAME, GEM::util::Logger::Level::info},
        {IO_LOGGER_NAME, GEM::util::Logger::Level::info},
        {MESH_LOGGER_NAME, GEM::util::Logger::Level::info},
        {SHADER_LOGGER_NAME, GEM::util::Logger::Level::info},
        {TEXTURE_LOGGER_NAME, GEM::util::Logger::Level::info}
    });

    const int initialWindowWidthPixels = 800;
    const int initialWindowHeightPixels = 600;
    const std::string windowTitle = "Application Name or someething, idrk ...";

    /* ------------------------------------ initialization ------------------------------------ */

    LOG_INFO("Configureing GLFW");

    // Initialize glfw
    glfwInit();

    // Configure glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GEM_GLFW_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GEM_GLFW_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE); // Doens't actually fix the red/black flickering effect
#ifdef ON_MAC
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // For mac os
#endif

    // Create the window
    GLFWmonitor* p_monitor = nullptr;
    GLFWwindow* p_sharedGlfwWindow = nullptr;
    GLFWwindow* p_glfwWindow = glfwCreateWindow(initialWindowWidthPixels, initialWindowHeightPixels, windowTitle.c_str(), p_monitor, p_sharedGlfwWindow);
    if (p_glfwWindow == nullptr) {
        LOG_CRITICAL("Failed to create GLFW window");
        glfwTerminate();
    }

    // Make the context the current context
    glfwMakeContextCurrent(p_glfwWindow);

    // Tell opengl to use the framebuffer size callback when the window is resized
    // This will be called once upon start up so we don't need to call glViewport right away
    glfwSetFramebufferSizeCallback(p_glfwWindow, framebufferSizeCallback);

    // Initialize glad
    // Give glad the function to load the address of the OS specific OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_CRITICAL("Failed to initialize GLAD");
        return -1;
    }

    // Mouse movement and scrolling (camera movement and perspective change)
    glfwSetInputMode(p_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(p_glfwWindow, mouseInputCallback);
    lastMouseXPos = initialWindowWidthPixels / 2;
    lastMouseYPos = initialWindowHeightPixels / 2;
    glfwSetScrollCallback(p_glfwWindow, mouseZoomCallback);

    // For 3d depth buffering
    glEnable(GL_DEPTH_TEST);

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

    GEM::Mesh mesh;

    std::vector<glm::vec3> cubePositions = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5)
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

    /* ------------------------------------ camera ------------------------------------ */

    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraLookVector = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUpVector = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 worldUpVector = glm::vec3(0.0f, 1.0f, 0.0f);

    /* ------------------------------------ actually drawing! yay :D ------------------------------------ */

    // For frame rate
    float deltaTime = 0.0f;
    float lastFrameStartTime = 0.0f;
    float currentFrameStartTime = glfwGetTime();

    // Determine what color we want to clear the screen to
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Create the render loop
    LOG_INFO("Starting render loop");
    while (!glfwWindowShouldClose(p_glfwWindow)) {
        // ----- Update frame rating stuff ----- //
        currentFrameStartTime = glfwGetTime();
        deltaTime = currentFrameStartTime - lastFrameStartTime;
        lastFrameStartTime = currentFrameStartTime;

        // ----- Get input ----- //

        processInput(p_glfwWindow);

        // ----- Rendering ----- //

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate and bind textures
        texture.activate();
        texture2.activate();

        // Set the active shader program
        shaderPrograms[0]->use();
        
        // Create the transformation matrices for getting object space -> world space -> view space -> clip space -> screen space
        
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraLookVector = glm::normalize(direction);

        cameraPosition = processCameraPosition(p_glfwWindow, deltaTime, cameraPosition, cameraLookVector, cameraUpVector, worldUpVector);

        glm::mat4 viewMatrix;
        viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraLookVector, cameraUpVector);
        shaderPrograms[0]->setUniformMat4("viewMatrix", viewMatrix);
        
        glm::mat4 projectionMatrix = glm::mat4(1.0f);
        projectionMatrix = glm::perspective(glm::radians(fovDegrees), 800.0f / 600.0f, 0.1f, 100.0f);
        shaderPrograms[0]->setUniformMat4("projectionMatrix", projectionMatrix);

        // Render the mesh many times, each time with a different position and rotation
        for (uint32_t i = 0; i < cubePositions.size(); ++i) {
            // Create the matrix for moving the mesh in world space
            glm::mat4 modelMatrix = glm::mat4(1.0f);

            // Move in world space
            modelMatrix = glm::translate(modelMatrix, cubePositions[i]);

            // Rotate an amount based on time over an axis changing over time
            float sign = (i % 2 == 0) ? -1 : 1;
            modelMatrix = glm::rotate(
                modelMatrix,
                static_cast<float>(glfwGetTime()) * glm::radians(sign * 45.0f) * (i+1),
                glm::normalize(glm::vec3(
                    0.5f,
                    static_cast<float>(glfwGetTime()) / 10 * (i+1) * 1.0f,
                    i
                ))
            );
        
            shaderPrograms[0]->setUniformMat4("modelMatrix", modelMatrix);
        
            mesh.draw();
        }

        // Check and call events and swap buffers
        glfwSwapBuffers(p_glfwWindow);
        glfwPollEvents();
    }

    // De allocate all resources once they've outlived their purpose
    LOG_INFO("Deallocating all resources");

    // Clear all previously allocated glfw resources
    LOG_INFO("Terminating GLFW");
    glfwTerminate();

    return 0;
}