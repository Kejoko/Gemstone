#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "util/macros.hpp"
#include "util/platform.hpp"
#include "util/logger/Logger.hpp"

namespace GEM {
    uint32_t compileShader(const char* shaderSource, const GLenum shaderType);
    uint32_t createShaderProgram(const uint32_t vertexShader, const uint32_t fragmentShader);
    std::vector<uint32_t> createShaderPrograms();
}

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

const char* fragmentShader2Source = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "\n"
    "void main() {\n"
    "    FragColor = vec4(0.2f, 0.5f, 1.0f, 1.0f);\n"
    "}\n"
    "\0"
;

/**
 * @brief Compile a shader given it's source code and the type of shader 
 * 
 * @note This function will throw an exception if the shader fails to compile
 * 
 * @param shaderSource The source code of the shader
 * @param shaderType The type of the shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc ...)
 * @return uint32_t The compiled shader
 */
uint32_t GEM::compileShader(const char* shaderSource, const GLenum shaderType) {
    GEM::Logger::trace("Copmiling shader of type " + std::to_string(shaderType));

    // Create a shader object, attach the shader source code, and compile
    uint32_t shader = glCreateShader(shaderType);

    // Give: The shader to compile, how many strings we are giving it as source,
    // the actual source code, and nullptr (why no length?)
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Check for successful compilation of the vertex shader
    int shaderCompilationSuccess;
    char shaderCompilationInfoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompilationSuccess);
    if (!shaderCompilationSuccess) {
        glGetShaderInfoLog(shader, sizeof(shaderCompilationInfoLog), nullptr, shaderCompilationInfoLog);
        const std::string errorMessage = "Vertex Shader failed to compile: " + std::string(shaderCompilationInfoLog);
        GEM::Logger::critical(errorMessage);
        throw std::invalid_argument(errorMessage);
    }

    return shader;
}

/**
 * @brief Create a shader program from the desired vertex and fragment shaders
 * 
 * @note This function will throw an exception if the linking fails
 * 
 * @param vertexShader A compiled vertex shader to link into the shader program
 * @param fragmentShader A compiled fragment shader to link into the shader program
 * @return uint32_t The linked shader program
 */
uint32_t GEM::createShaderProgram(const uint32_t vertexShader, const uint32_t fragmentShader) {
    // Create a shader program to link the vertex and fragment shaders and attach the compiled shaders
    uint32_t shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check that shader program linking was successful
    int shaderProgramLinkSuccess;
    char shaderProgramLinkInfoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shaderProgramLinkSuccess);
    if (!shaderProgramLinkSuccess) {
        glGetProgramInfoLog(shaderProgram, sizeof(shaderProgramLinkInfoLog), nullptr, shaderProgramLinkInfoLog);
        const std::string errorMessage = "Shader program failed to link: " + std::string(shaderProgramLinkInfoLog);
        GEM::Logger::critical(errorMessage);
        throw std::invalid_argument(errorMessage);
    }

    return shaderProgram;
}

/**
 * @brief Create a vector of shader programs. Use the vertex shader we have defined and the two fragment shaders.
 * 
 * @note This will throw an exception if the it fails to compile a shader or create a shader program
 * 
 * @return std::vector<uint32_t> A vector of shader programs to use
 */
std::vector<uint32_t> GEM::createShaderPrograms() {
    GEM::Logger::trace("Creating shader programs");

    // Create the shaders
    uint32_t vertexShader = GEM::compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    uint32_t fragmentShader = GEM::compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    uint32_t fragmentShader2 = GEM::compileShader(fragmentShader2Source, GL_FRAGMENT_SHADER);

    // Create the shader programs
    uint32_t shaderProgram = GEM::createShaderProgram(vertexShader, fragmentShader);
    uint32_t shaderProgram2 = GEM::createShaderProgram(vertexShader, fragmentShader2);

    // Delete the shaders after we've linked them to the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);

    return {shaderProgram, shaderProgram2};
}