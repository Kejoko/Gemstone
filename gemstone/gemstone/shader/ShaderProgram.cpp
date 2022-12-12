#include "gemstone/shader/ShaderProgram.hpp"

#include <string>

#include <glad/glad.h>

#include "util/macros.hpp"
#include "util/logger/Logger.hpp"

#include "gemstone/shader/logger.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the Shader class uses
 * 
 */
const std::string GEM::ShaderProgram::LOGGER_NAME = SHADER_LOGGER_NAME;

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
    LOG_FUNCTION_CALL_INFO("vertex id {} , fragment id {}", vertexShaderID, fragmentShaderID);

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
        LOG_CRITICAL(errorMessage);
        throw std::invalid_argument(errorMessage);
    }

    LOG_TRACE("Successfully linked shader program with id {}", shaderProgramID);

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
    m_id(GEM::ShaderProgram::createShaderProgram(m_vertexShader.getID(), m_fragmentShader.getID()))
{}

/**
 * @brief Destroy the GEM::ShaderProgram::ShaderProgram object and use glDeleteProgram to delete this shader
 * program
 */
GEM::ShaderProgram::~ShaderProgram() {
    LOG_FUNCTION_CALL_TRACE("id {}", m_id);
    glDeleteProgram(m_id);
}

/**
 * @brief Set this shader as the active shader using glUseProgram
 */
void GEM::ShaderProgram::use() const {
    glUseProgram(m_id);
}

/**
 * @brief Set the value of a uniform representing a bool or vector of bools within the glsl shader
 * 
 * @param name The name of the uniform to set
 * @param value(s) The values to set the uniform to
 */

void GEM::ShaderProgram::setUniformBool(const std::string& uniformName, const bool value) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform1i(uniformLocation, static_cast<int32_t>(value));
}
void GEM::ShaderProgram::setUniformBVec2(const std::string& uniformName, const std::array<bool, 2>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform2i(uniformLocation, static_cast<int32_t>(values[0]), static_cast<int32_t>(values[1]));
}
void GEM::ShaderProgram::setUniformBVec3(const std::string& uniformName, const std::array<bool, 3>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform3i(uniformLocation, static_cast<int32_t>(values[0]), static_cast<int32_t>(values[1]), static_cast<int32_t>(values[2]));
}
void GEM::ShaderProgram::setUniformBVec4(const std::string& uniformName, const std::array<bool, 4>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform4i(uniformLocation, static_cast<int32_t>(values[0]), static_cast<int32_t>(values[1]), static_cast<int32_t>(values[2]), static_cast<int32_t>(values[3]));
}

/**
 * @brief Set the value of a uniform representing a int or vector of ints within the glsl shader
 * 
 * @param name The name of the uniform to set
 * @param value(s) The values to set the uniform to
 */

void GEM::ShaderProgram::setUniformInt(const std::string& uniformName, const int32_t value) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform1i(uniformLocation, value);
}
void GEM::ShaderProgram::setUniformIVec2(const std::string& uniformName, const std::array<int32_t, 2>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform2i(uniformLocation, values[0], values[1]);
}
void GEM::ShaderProgram::setUniformIVec3(const std::string& uniformName, const std::array<int32_t, 3>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform3i(uniformLocation, values[0], values[1], values[2]);
}
void GEM::ShaderProgram::setUniformIVec4(const std::string& uniformName, const std::array<int32_t, 4>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform4i(uniformLocation, values[0], values[1], values[2], values[3]);
}

/**
 * @brief Set the value of a uniform representing a uint or vector of uints within the glsl shader
 * 
 * @param name The name of the uniform to set
 * @param value(s) The values to set the uniform to
 */

void GEM::ShaderProgram::setUniformUInt(const std::string& uniformName, const uint32_t value) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform1ui(uniformLocation, value);
}
void GEM::ShaderProgram::setUniformUVec2(const std::string& uniformName, const std::array<uint32_t, 2>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform2ui(uniformLocation, values[0], values[1]);
}
void GEM::ShaderProgram::setUniformUVec3(const std::string& uniformName, const std::array<uint32_t, 3>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform3ui(uniformLocation, values[0], values[1], values[2]);
}
void GEM::ShaderProgram::setUniformUVec4(const std::string& uniformName, const std::array<uint32_t, 4>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform4ui(uniformLocation, values[0], values[1], values[2], values[3]);
}

/**
 * @brief Set the value of a uniform representing a float or vector of floats within the glsl shader
 * 
 * @param name The name of the uniform to set
 * @param value(s) The values to set the uniform to
 */

void GEM::ShaderProgram::setUniformFloat(const std::string& uniformName, const float value) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform1f(uniformLocation, value);
}
void GEM::ShaderProgram::setUniformVec2(const std::string& uniformName, const std::array<float, 2>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform2f(uniformLocation, values[0], values[1]);
}
void GEM::ShaderProgram::setUniformVec3(const std::string& uniformName, const std::array<float, 3>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform3f(uniformLocation, values[0], values[1], values[2]);
}
void GEM::ShaderProgram::setUniformVec4(const std::string& uniformName, const std::array<float, 4>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform4f(uniformLocation, values[0], values[1], values[2], values[3]);
}

/* ------------------------------ private member functions ------------------------------ */
