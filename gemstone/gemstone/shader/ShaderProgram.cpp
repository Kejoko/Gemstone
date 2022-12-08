#include "gemstone/shader/ShaderProgram.hpp"

#include <string>

#include <glad/glad.h>

#include "util/macros.hpp"
#include "util/logger/Logger.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the Shader class uses
 * 
 */
const std::string GEM::ShaderProgram::LOGGER_NAME = "SHADER";

/* ------------------------------ private static variables ------------------------------ */

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/**
 * @brief Create a shader program from the desired vertex and fragment shaders
 * 
 * @note This function will throw an exception if the linking fails
 * 
 * @param vertexShaderID The id of a compiled vertex shader to link into the shader program
 * @param fragmentShaderID The id of a compiled fragment shader to link into the shader program
 * @return uint32_t The id of the linked shader program
 */
uint32_t GEM::ShaderProgram::createShaderProgram(const uint32_t vertexShaderID, const uint32_t fragmentShaderID) {
    LOG_FUNCTION_CALL_INFO("vertex id " + std::to_string(vertexShaderID) + " , fragment id " + std::to_string(fragmentShaderID));

    // Create a shader program to link the vertex and fragment shaders and attach the compiled shaders
    uint32_t shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);
    glLinkProgram(shaderProgramID);

    // Check that shader program linking was successful
    int shaderProgramLinkSuccess;
    char shaderProgramLinkInfoLog[512];
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &shaderProgramLinkSuccess);
    if (!shaderProgramLinkSuccess) {
        glGetProgramInfoLog(shaderProgramID, sizeof(shaderProgramLinkInfoLog), nullptr, shaderProgramLinkInfoLog);
        const std::string errorMessage = "Shader program failed to link:\n" + std::string(shaderProgramLinkInfoLog);
        GEM::Logger::critical(errorMessage);
        throw std::invalid_argument(errorMessage);
    }

    GEM::Logger::trace("Successfully linked shader program with id " + std::to_string(shaderProgramID));

    return shaderProgramID;
}

/* ------------------------------ public member functions ------------------------------ */

/**
 * @brief Construct a new Shader object from the given vertex and fragment shaders
 * 
 * @param vertexShaderSource The string containing the source for the vertex shader
 * @param fragmentShaderSource The string containing the source for the fragment shader
 */
GEM::ShaderProgram::ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) :
    m_vertexShader(vertexShaderSource, GL_VERTEX_SHADER),
    m_fragmentShader(fragmentShaderSource, GL_FRAGMENT_SHADER),
    m_shaderID(GEM::ShaderProgram::createShaderProgram(m_vertexShader.getID(), m_fragmentShader.getID()))
{}

GEM::ShaderProgram::~ShaderProgram() {
    LOG_FUNCTION_CALL_TRACE("id " + std::to_string(m_shaderID));
    glDeleteProgram(m_shaderID);
}

/**
 * @brief Set this shader as the active shader using glUseProgram
 */
void GEM::ShaderProgram::use() const {
    glUseProgram(m_shaderID);
}

/* ------------------------------ private member functions ------------------------------ */
