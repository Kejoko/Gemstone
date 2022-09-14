#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "util/platform.hpp"
#include "util/logger/Logger.hpp"
#include "gemstone/core.hpp"
#include "application/core.hpp"

/**
 * @brief A function that gets called every time the window is resized
 * 
 * @param p_glfwWindow A pointer to the glfw window
 * @param updatedWindowWidthPixels The updated width of the window in pixels
 * @param updatedWindowHeightPixels The updated height of the window in pixels
 */
void framebufferSizeCallback(GLFWwindow* p_glfwWindow, int updatedWindowWidthPixels, int updatedWindowHeightPixels) {
    glViewport(0, 0, updatedWindowWidthPixels, updatedWindowHeightPixels);
}

int main(int argc, char* argv[]) {
    ASSERT_GEM_VERSION();
    ASSERT_APP_VERSION();

    GEM::Logger::init();

    const int initialWindowWidthPixels = 800;
    const int initialWindowHeightPixels = 600;
    const std::string windowTitle = "Application Name or someething, idrk ...";

    // Initialize glfw
    glfwInit();

    // Configure glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GEM_GLFW_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GEM_GLFW_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef ON_MAC
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE); // For mac os
#endif

    // Create the window
    GLFWmonitor* p_monitor = nullptr;
    GLFWwindow* p_sharedGlfwWindow = nullptr;
    GLFWwindow* p_glfwWindow = glfwCreateWindow(initialWindowWidthPixels, initialWindowHeightPixels, windowTitle.c_str(), p_monitor, p_sharedGlfwWindow);
    if (p_glfwWindow == nullptr) {
        GEM::Logger::critical("Failed to create GLFW window");
        glfwTerminate();
    }

    // Make the context the current context
    glfwMakeContextCurrent(p_glfwWindow);

    // Initialize glad
    // Give glad the function to load the address of the OS specific OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        GEM::Logger::critical("Failed to initialize GLAD");
        return -1;
    }

    // Tell opengl the size of the rendering window
    glViewport(0, 0, initialWindowWidthPixels, initialWindowHeightPixels);

    // Tell opengl to use the framebuffer size callback when the window is resized
    glfwSetFramebufferSizeCallback(p_glfwWindow, framebufferSizeCallback);

    // Create the render loop
    while (!glfwWindowShouldClose(p_glfwWindow)) {
        glfwSwapBuffers(p_glfwWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}