#include <iostream>
#include <string>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "util/platform.hpp"
#include "util/logger/Logger.hpp"
#include "gemstone/core.hpp"
#include "application/core.hpp"

const char* vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "\n"
    "void main() {\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n"
    "\0"
;

const char* fragmentShaderSource = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "\n"
    "void main() {\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n"
    "\0"
;


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

/**
 * @brief Get user input
 * 
 * @param p_glfwWindow A pointer to the glfw window
 */
void processInput(GLFWwindow* p_glfwWindow) {
    // End it all! ... if the user presses the escape key ...
    if (glfwGetKey(p_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(p_glfwWindow, true);
    }
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

    // The vertices we are using to draw the triangle
    const float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    // Create a vertex array object to store all of our vertex attribute's informations
    // When you have multiple objects you want to draw you first generate and configure
    // all of the VAOs (and consequently the required VBO and attribute pointers) to be
    // stored for later use
    // The moement we want to draw one of our objects we bind the corresponding VAO,
    // draw it, then unbind the VAO
    unsigned int vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Create the vertex buffer object and bind it so any call on GL_ARRAY_BUFFER target
    // will be used to configure the vertex buffer object
    unsigned int vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

    // Copy the vertices into the currently bound vertex buffer
    // Give: type of buffer to copy, size of data in bytes, the actual data, and how
    // we want the graphics card to manage the given data
    // Using GL_STATIC_DRAW because data does not change and is drawn alot
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

    // Create a shader object, attach the shader source code, and compile
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Give: The vertex shader to compile, how many strings we are giving it as source,
    // the actual source code, and nullptr (why no length?)
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Check for successful compilation of the vertex shader
    int vertexShaderCompilationSuccess;
    char vertexShaderCompilationInfoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderCompilationSuccess);
    if (!vertexShaderCompilationSuccess) {
        glGetShaderInfoLog(vertexShader, sizeof(vertexShaderCompilationInfoLog), nullptr, vertexShaderCompilationInfoLog);
        GEM::Logger::critical("Vertex Shader failed to compile: " + std::string(vertexShaderCompilationInfoLog));
        return 1;
    }

    // Fragment shader time
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check for successful compilation of the fragment shader
    int fragmentShaderCompilationSuccess;
    char fragmentShaderCompilationInfoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderCompilationSuccess);
    if (!fragmentShaderCompilationSuccess) {
        glGetShaderInfoLog(vertexShader, sizeof(fragmentShaderCompilationInfoLog), nullptr, fragmentShaderCompilationInfoLog);
        GEM::Logger::critical("Fragment Shader failed to compile: " + std::string(fragmentShaderCompilationInfoLog));
        return 1;
    }

    // Create a shader program to link the vertex and fragment shaders and attach the compiled shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check that shader program linking was successful
    int shaderProgramLinkSuccess;
    char shaderProgramLinkInfoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderProgramLinkSuccess);
    if (!shaderProgramLinkSuccess) {
        glGetProgramInfoLog(shaderProgram, sizeof(shaderProgramLinkInfoLog), nullptr, shaderProgramLinkInfoLog);
        GEM::Logger::critical("Shader program failed to link: " + std::string(shaderProgramLinkInfoLog));
        return 1;
    }

    // Delete the shaders after we've linked them to the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Determine what color we want to clear the screen to
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Create the render loop
    while (!glfwWindowShouldClose(p_glfwWindow)) {
        // ------------------
        // Get input
        // ------------------
        processInput(p_glfwWindow);

        // ------------------
        // Render
        // ------------------
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayObject);
        glDrawArrays(
            GL_TRIANGLES,   // The type of primitive
            0,              // The starting index of the vertex array we'd like to draw
            3               // How many vertices we want to draw
        );


        // Checdk and call events and swap buffers
        glfwSwapBuffers(p_glfwWindow);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}