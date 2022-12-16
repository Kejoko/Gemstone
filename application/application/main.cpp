#include <cmath>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include "util/macros.hpp"
#include "util/platform.hpp"
#include "util/logger/Logger.hpp"

#include "gemstone/core.hpp"
#include "gemstone/shader/logger.hpp"
#include "gemstone/shader/ShaderProgram.hpp"

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

    GEM::util::Logger::registerLoggers({
        {GENERAL_LOGGER_NAME, GEM::util::Logger::Level::trace},
        {SHADER_LOGGER_NAME, GEM::util::Logger::Level::trace}
    });

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

    /* ------------------------------------ shader stuff ------------------------------------ */

    LOG_INFO("Managing shaders");

    std::vector<std::shared_ptr<GEM::ShaderProgram>> shaderPrograms;
    try {
        shaderPrograms.push_back(std::make_shared<GEM::ShaderProgram>(vertexShaderSource, fragmentShaderSource));
        shaderPrograms.push_back(std::make_shared<GEM::ShaderProgram>(vertexShaderSource, fragmentShader2Source));
    } catch (const std::exception& ex) {
        LOG_CRITICAL("Caught exception when trying to create shaders:\n" + std::string(ex.what()));
        return 1;
    }

    /* ------------------------------------ vertices, indices, and EBO for drawing a rectangle ------------------------------------ */

    LOG_INFO("Creating VAO, VBO, EBO");

    // // The vertices for each corner of the rectangle
    // const float rectangleVertices[] = {
    //     // Positions            colors
        
    //     // Middle rectangle
    //      0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   // top right (red)
    //      0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   // bottom right (blue)
    //     -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   // bottom left (green)
    //     -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,   // top left (yellow)

    //     // Right triangle Point
    //      0.9f,  0.0f, 0.0f,     1.0f, 1.0f, 1.0f,   // white

    //     // Top Triangle point
    //      0.0f,  0.9f, 0.0f,     0.0f, 0.0f, 0.0f    // black
    // };

    // // The indices we are using to reference the rectangle's vertices for the two triangles we need to draw
    // const uint32_t rectangleIndices[] = {
    //     // Middle rectangle
    //     0, 1, 3, // first triangle
    //     1, 2, 3, // second triangle

    //     // Right triangle
    //     0, 4, 1,

    //     // Top triangle
    //     3, 5, 0
    // };

    // The vertices for each corner of the rectangle
    const float rectangleVertices[] = {
        // Positions            colors              texture coords
         0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    // The indices we are using to reference the rectangle's vertices for the two triangles we need to draw
    const uint32_t rectangleIndices[] = {
        // Middle rectangle
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
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
    const int p_vertexPositionAttribute = 0;
    glVertexAttribPointer(
        p_vertexPositionAttribute,  // The vertex attribute we want to configure (0 because we used 'location = 0' in our vertex shader, we want to pass this to our vertex shader)
        3,                          // The size of the vertex attribute (vec3 has 3 values)
        GL_FLOAT,                   // Data type (vec<N> in GLSL consists of floating point values)
        GL_FALSE,                   // To normalize or not to normalize (when using int types this sets the data to -1, 0, or 1 upon conversion to float)
        8 * sizeof(float),          // The stride length, the space between consecutive vertex attributes (each vertex and color is 3 floats so the starts of each vertex differs by 6 floats)
        static_cast<void*>(0)       // The offset of where the position data begins in the buffer (0 because position data starts at the beggining of the buffer)
    );
    glEnableVertexAttribArray(p_vertexPositionAttribute);

    // Configure the color vertex attribute the same way we did the positions
    const int p_vertexColorAttribute = 1;
    glVertexAttribPointer(
        p_vertexColorAttribute,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),          // Stride length of 8 becasue 3 position + 3 color + 2 texture
        (void*)(3 * sizeof(float))  // Offset 3 floats into the array because the first 3 values are position data
    );
    glEnableVertexAttribArray(p_vertexColorAttribute);

    // Configure the texture vertex attribute like the color and positions
    const int p_vertexTextureAttribute = 2;
    glVertexAttribPointer(
        p_vertexTextureAttribute,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),          // Stride length of 8 becasue 3 position + 3 color + 2 texture
        (void*)(6 * sizeof(float))  // Offset 6 floats into the array because 3 position and 3 color preceeding this
    );
    glEnableVertexAttribArray(p_vertexTextureAttribute);

    // The call to glVertexAttribPointer registered the VBO as te vertex attribute's bound VBO, so we can safely unbind afterwards
    // We must not unbind the EBO because it is stored in the VAO
    // We can unbind the VAO so other VAO calls don't accidentally modify this VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* ------------------------------------ textures ------------------------------------ */

    LOG_INFO("Loading textures");

    // Create the texture in open gl
    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set the texture wrapping method
    // Don't forget to set the border color with glTexParameterfv if we use GL_CLAMP_TO_BORDER for wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set the texture filtering method (nearest or linear interpolation) for both magnifying (scaling up the
    // texture) and minifying (scaling down the texture) operations
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Create and use the mipmaps. This isn't importatnt for the mag filter because it only applies when upscaling
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the texture
    // For the issue with loading pngs:
    // https://stackoverflow.com/questions/23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors
    LOG_TRACE("Actually loading texture 1");
    int textureWidth;
    int textureHeight;
    int textureChannelCount;
    stbi_set_flip_vertically_on_load(true);
    uint8_t* p_textureData = stbi_load("../application/assets/textures/wooden_container.jpg", &textureWidth, &textureHeight, &textureChannelCount, 0);
    if (!p_textureData) {
        LOG_CRITICAL("Failed to glload texture 1");
        return -1;
    }

    LOG_TRACE("Generate the opengl texture 1 using the loaded data");
    glTexImage2D(
        GL_TEXTURE_2D,      // The texture target (we are bound to 2d due to the glBindTexture call)
        0,                  // The mipmap level for which we want to create a texture for
        GL_RGB,             // What kind of format we want to store the texture
        textureWidth,       // Set the width of the resulting texture
        textureHeight,      // Set the height of the resulting texture
        0,                  // Should always be 0 -- legacy stuff
        GL_RGB,             // Format of the source image
        GL_UNSIGNED_BYTE,   // Data type of the source image
        p_textureData       // The actual image data
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free the image memory now that we have generated the texture
    stbi_image_free(p_textureData);

    uint32_t texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    LOG_TRACE("Actually loading texture 2");
    int texture2Width;
    int texture2Height;
    int texture2ChannelCount;
    stbi_set_flip_vertically_on_load(true);
    uint8_t* p_texture2Data = stbi_load("../application/assets/textures/awesomeface.png", &texture2Width, &texture2Height, &texture2ChannelCount, 0);
    if (!p_texture2Data) {
        LOG_CRITICAL("Failed to glload texture 2");
        return -1;
    }
    
    LOG_TRACE("Generate the opengl texture 2 using the loaded data");
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        texture2Width,
        texture2Height,
        0,
        GL_RGBA,            // RGBA instead of RGB because pngs have alpha (transparency)
        GL_UNSIGNED_BYTE,
        p_texture2Data
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(p_texture2Data);

    // Set the uniforms in the shader to the correct textures
    // match the value we set it to as the same value as whichever active texture it is
    shaderPrograms[0]->use();
    shaderPrograms[0]->setUniformInt("ourTexture", 0);  
    shaderPrograms[0]->setUniformInt("ourTexture2", 1);


    /* ------------------------------------ actually drawing! yay :D ------------------------------------ */

    // Determine what color we want to clear the screen to
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Create the render loop
    LOG_INFO("Starting render loop");
    while (!glfwWindowShouldClose(p_glfwWindow)) {
        // ----- Get input ----- //

        processInput(p_glfwWindow);

        // ----- Rendering ----- //

        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(vertexArrayObject);

        // Rectangle

        shaderPrograms[0]->use();

        glDrawElements(
            GL_TRIANGLES,       // The type of primitive
            6,                  // The number of elements to be rendered
            GL_UNSIGNED_INT,    // The type of the values in the indices
            0                   // The offset into the EBO
        );

        // // Outside triangles

        // // Determine the color we want to use
        // float timeValue = glfwGetTime();
        // float greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;

        // // We don't need to use the shader program to find the uniform but we do need
        // // to use the shader program to assign it, because it assigns to the current shader program
        // shaderPrograms[1]->use();
        // shaderPrograms[1]->setUniformVec4("ourColor", {0.0f, greenValue, 0.0f, 1.0f});
        
        // glDrawElements(
        //     GL_TRIANGLES,                   // The type of primitive
        //     6,                              // The number of elements to be rendered
        //     GL_UNSIGNED_INT,                // The type of the values in the indices
        //     (void*)(6 * sizeof(uint32_t))   // The offset into the EBO
        // );

        // Check and call events and swap buffers
        glfwSwapBuffers(p_glfwWindow);
        glfwPollEvents();
    }

    // De allocate all resources once they've outlived their purpose
    LOG_INFO("Deallocating all resources");
    glDeleteVertexArrays(1, &vertexArrayObject);
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);

    // Clear all previously allocated glfw resources
    LOG_INFO("Terminating GLFW");
    glfwTerminate();

    return 0;
}