#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "util/macros.hpp"
#include "util/platform.hpp"
#include "util/logger/Logger.hpp"
#include "gemstone/core.hpp"
#include "application/core.hpp"
#include "application/shaders.hpp"

/**
 * @brief A function that gets called every time the window is resized
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

    // End it all! ... if the user presses the escape key ...
    if (glfwGetKey(p_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(p_glfwWindow, true);
    }
}


int main(int argc, char* argv[]) {
    UNUSED(argc);
    UNUSED(argv);


    ASSERT_GEM_VERSION();
    ASSERT_APP_VERSION();

    GEM::Logger::init();

    const int initialWindowWidthPixels = 800;
    const int initialWindowHeightPixels = 600;
    const std::string windowTitle = "Application Name or someething, idrk ...";

    /* ------------------------------------ initialization ------------------------------------ */

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
        GEM::Logger::critical("Failed to create GLFW window");
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
        GEM::Logger::critical("Failed to initialize GLAD");
        return -1;
    }

    /* ------------------------------------ shader stuff ------------------------------------ */

    GEM::Logger::info("Managing shaders");

    std::vector<uint32_t> shaderPrograms;
    try {
        shaderPrograms = GEM::createShaderPrograms();
    } catch (const std::exception& ex) {
        GEM::Logger::critical("Caught exception when trying to create shader programs: " + std::string(ex.what()));
        return 1;
    }

    /* ------------------------------------ vertices, indices, and EBO for drawing a rectangle ------------------------------------ */

    GEM::Logger::info("Creating VAO, VBO, EBO");

    // The vertices for each corner of the rectangle
    const float rectangleVertices[] = {
        // Middle rectangle
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,  // top left

        // Right triangle Point
         0.9f,  0.0f, 0.0f,

        // Top Triangle point
         0.0f,  0.9f, 0.0f
    };

    // The indices we are using to reference the rectangle's vertices for the two triangles we need to draw
    const uint32_t rectangleIndices[] = {
        // Middle rectangle
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle

        // Right triangle
        0, 4, 1,

        // Top triangle
        3, 5, 0
    };

    // Create a vertex array object to store all of our vertex attribute's informations
    // When you have multiple objects you want to draw you first generate and configure
    // all of the VAOs (and consequently the required VBO and attribute pointers) to be
    // stored for later use
    // The moment we want to draw one of our objects we bind the corresponding VAO,
    // draw it, then unbind the VAO
    uint32_t vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Create the vertex buffer object and bind it so any call on GL_ARRAY_BUFFER target
    // will be used to configure the vertex buffer object
    uint32_t vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    // Copy the vertices into the currently bound vertex buffer
    // Give: type of buffer to copy, size of data in bytes, the actual data, and how
    // we want the graphics card to manage the given data
    // Using GL_STATIC_DRAW because data does not change and is drawn alot
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), rectangleVertices, GL_STATIC_DRAW);

    // Create the element buffer object (EBO)
    uint32_t elementBufferObject;
    glGenBuffers(1, &elementBufferObject);

    // Similar to the VBO, bind the EBO and copy the indices into the buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleIndices), rectangleIndices, GL_STATIC_DRAW);

    // Tell OpenGL how it should interpret the vertex data
    // First two parameters should be configured to match the information in our vertex shader, it seems
    const int p_vertexAttribute = 0;
    glVertexAttribPointer(
        p_vertexAttribute,      // The vertex attribute we want to configure (0 because we used 'location = 0' in our vertex shader, we want to pass this to our vertex shader)
        3,                      // The size of the vertex attribute (vec3 has 3 values)
        GL_FLOAT,               // Data type (vec<N> in GLSL consists of floating point values)
        GL_FALSE,               // To normalize or not to normalize (when using int types this sets the data to -1, 0, or 1 upon conversion to float)
        3 * sizeof(float),      // The stride length, the space between consecutive vertex attributes (each vertex is 3 floats so the starts of each vertex differs by 3 floats)
        static_cast<void*>(0)   // The offset of where the position data begins in the buffer (0 because position data starts at the beggining of the buffer)
    );
    glEnableVertexAttribArray(p_vertexAttribute);

    // The call to glVertexAttribPointer registered the VBO as te vertex attribute's bound VBO, so we can safely unbind afterwards
    // We must not unbind the EBO because it is stored in the VAO
    // We can unbind the VAO so other VAO calls don't accidentally modify this VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* ------------------------------------ actually drawing! yay :D ------------------------------------ */

    // Determine what color we want to clear the screen to
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Create the render loop
    GEM::Logger::info("Starting render loop");
    while (!glfwWindowShouldClose(p_glfwWindow)) {
        // ------------------
        // Get input
        // ------------------
        processInput(p_glfwWindow);

        // ------------------
        // Render
        // ------------------
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vertexArrayObject);

        glUseProgram(shaderPrograms[0]);
        glDrawElements(
            GL_TRIANGLES,       // The type of primitive
            6,                  // The number of elements to be rendered
            GL_UNSIGNED_INT,    // The type of the values in the indices
            0                   // The offset into the EBO
        );

        glUseProgram(shaderPrograms[1]);
        glDrawElements(
            GL_TRIANGLES,                   // The type of primitive
            6,                              // The number of elements to be rendered
            GL_UNSIGNED_INT,                // The type of the values in the indices
            (void*)(6 * sizeof(uint32_t))   // The offset into the EBO
        );

        // Check and call events and swap buffers
        glfwSwapBuffers(p_glfwWindow);
        glfwPollEvents();
    }

    // De allocate all resources once they've outlived their purpose
    GEM::Logger::info("Deallocating all resources");
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);
    glDeleteProgram(shaderPrograms[0]);

    // Clear all previously allocated glfw resources
    GEM::Logger::info("Terminating GLFW");
    glfwTerminate();

    return 0;
}