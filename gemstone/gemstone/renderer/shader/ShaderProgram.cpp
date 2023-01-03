#include <string>
#include <utility>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "util/macros.hpp"
#include "util/logger/Logger.hpp"

#include "gemstone/renderer/texture/Texture.hpp"
#include "gemstone/renderer/shader/logger.hpp"
#include "gemstone/renderer/shader/CompiledShader.hpp"
#include "gemstone/renderer/shader/ShaderProgram.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the Shader class uses
 */
const std::string GEM::Renderer::ShaderProgram::LOGGER_NAME = SHADER_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/**
 * @brief A map of vertex and fragment shader IDs to linked shader programs
 */
std::map<std::pair<uint32_t, uint32_t>, GEM::Renderer::ShaderProgram::Info> GEM::Renderer::ShaderProgram::shaderProgramIDMap;

/* ------------------------------ public static functions ------------------------------ */

/* ------------------------------ private static functions ------------------------------ */

/**
 * @brief Add a newly linked shader program to the map for it to be found in the future
 * 
 * @param compiledIDs The ids of the compiled shaders
 * @param shaderProgramID The id of the linked shader program
 */
void GEM::Renderer::ShaderProgram::addShaderProgramToMap(const std::pair<uint32_t, uint32_t>& compiledIDs, const uint32_t shaderProgramID) {
    LOG_FUNCTION_ENTRY_TRACE("vertex id {} , fragment id {} , shader program id {}", compiledIDs.first, compiledIDs.second, shaderProgramID);

    GEM::Renderer::ShaderProgram::shaderProgramIDMap.insert({compiledIDs, {shaderProgramID, 0}});

    GEM::Renderer::ShaderProgram::incrementShaderProgramUseCount(compiledIDs);
}

/**
 * @brief Increment the use count for a given linked shader program so we know how many things are using it
 * 
 * @param compiledIDs The ids of the compiled shaders
 */
void GEM::Renderer::ShaderProgram::incrementShaderProgramUseCount(const std::pair<uint32_t, uint32_t>& compiledIDs) {
    LOG_FUNCTION_ENTRY_TRACE("vertex id {} , fragment id {}", compiledIDs.first, compiledIDs.second);

    GEM::Renderer::ShaderProgram::Info info = GEM::Renderer::ShaderProgram::shaderProgramIDMap[compiledIDs];

    LOG_TRACE("Use count for shader program with id {} was: {}", info.id, info.useCount);

    info.useCount += 1;

    LOG_TRACE("Use count for shader program with id {} is now: {}", info.id, info.useCount);

    GEM::Renderer::ShaderProgram::shaderProgramIDMap[compiledIDs] = info;
}

/**
 * @brief Decrement the use count for a given linked shader program so we know how many things are using it. If the
 * use count hits 0 or below we will remove the linked shader program from the map and delete it because nothing
 * cares about it anymore
 * 
 * @param compiledIDs The ids of the compiled shaders
 */
void GEM::Renderer::ShaderProgram::decrementShaderProgramUseCount(const std::pair<uint32_t, uint32_t>& compiledIDs) {
    LOG_FUNCTION_ENTRY_TRACE("vertex id {} , fragment id {}", compiledIDs.first, compiledIDs.second);

    GEM::Renderer::ShaderProgram::Info info = GEM::Renderer::ShaderProgram::shaderProgramIDMap[compiledIDs];

    LOG_TRACE("Use count for shader program with id {} was: {}", info.id, info.useCount);

    info.useCount -= 1;

    LOG_TRACE("Use count for shader program with id {} is now: {}", info.id, info.useCount);

    // If the shader is still in use then update the use count
    if (info.useCount > 0) {
        LOG_TRACE("Updating use count for shader program with id {}", info.id);
        GEM::Renderer::ShaderProgram::shaderProgramIDMap[compiledIDs] = info;
        return;
    }

    LOG_TRACE("Erasing shader program with id {} from map", info.id);
    GEM::Renderer::ShaderProgram::shaderProgramIDMap.erase(compiledIDs);

    LOG_TRACE("Deleting shader program with id {}", info.id);
    glDeleteProgram(info.id);
}

/**
 * @brief Check if a shader program is linked given the compiled shader ids
 * 
 * @param compiledIDs The ids of the compiled shaders
 * @return true There already exists a linked shader program for the compiled shader ids
 * @return false There does not yet exist a linked shader program for the compiled shader ids
 */
bool GEM::Renderer::ShaderProgram::shaderProgramIsLinked(const std::pair<uint32_t, uint32_t>& compiledIDs) {
    LOG_FUNCTION_ENTRY_TRACE("vertex id {} , fragment id {}", compiledIDs.first, compiledIDs.second);

    bool isLinked = GEM::Renderer::ShaderProgram::shaderProgramIDMap.count(compiledIDs) > 0;

    if (isLinked) {
        LOG_DEBUG("Found shader program for vertex id {} and fragment id {}", compiledIDs.first, compiledIDs.second);
    }

    return isLinked;
}

/**
 * @brief Given compiled shaders which we know are linked into a shader program already, get the id of that shader program
 * 
 * @param compiledIDs The ids of the compiled shaders
 * @return uint32_t The id of the linked shader program
 */
uint32_t GEM::Renderer::ShaderProgram::getShaderProgramID(const std::pair<uint32_t, uint32_t>& compiledIDs) {
    LOG_FUNCTION_ENTRY_TRACE("vertex id {} , fragment id {}", compiledIDs.first, compiledIDs.second);

    uint32_t shaderProgramID = GEM::Renderer::ShaderProgram::shaderProgramIDMap[compiledIDs].id;

    LOG_TRACE("Got shader program id {}", shaderProgramID);

    return shaderProgramID;
}

/**
 * @brief Create a shader program from the desired vertex and fragment shaders
 * 
 * @note This function will throw an exception if the linking fails
 * 
 * @param vertexShaderID The id of a compiled vertex shader to link into the shader program
 * @param fragmentShaderID The id of a compiled fragment shader to link into the shader program
 * @return uint32_t The id of the linked shader program
 */
uint32_t GEM::Renderer::ShaderProgram::createShaderProgram(const uint32_t vertexShaderID, const uint32_t fragmentShaderID) {
    LOG_FUNCTION_CALL_INFO("vertex id {} , fragment id {}", vertexShaderID, fragmentShaderID);

    // Before we try to link a new shader program, we should see if there is already one linked using the same
    // compiled shaders
    const std::pair<uint32_t, uint32_t> compiledIDs = {vertexShaderID, fragmentShaderID};
    if (GEM::Renderer::ShaderProgram::shaderProgramIsLinked(compiledIDs)) {
        // We found one given the vertex and fragment shaders, yay!
        uint32_t shaderProgramID = GEM::Renderer::ShaderProgram::getShaderProgramID(compiledIDs);

        GEM::Renderer::ShaderProgram::incrementShaderProgramUseCount(compiledIDs);

        LOG_DEBUG("Successfully found linked shader program with id {}", shaderProgramID);

        return shaderProgramID;
    }

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

    GEM::Renderer::ShaderProgram::addShaderProgramToMap(compiledIDs, shaderProgramID);

    LOG_DEBUG("Successfully linked shader program with id {}", shaderProgramID);

    return shaderProgramID;
}

/* ------------------------------ public member functions ------------------------------ */

/**
 * @brief Construct a new Shader object from the given vertex and fragment shaders
 * 
 * @param vertexShaderSource The string containing the source for the vertex shader
 * @param fragmentShaderSource The string containing the source for the fragment shader
 */
GEM::Renderer::ShaderProgram::ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) :
    m_vertexShader(vertexShaderSource, GL_VERTEX_SHADER),
    m_fragmentShader(fragmentShaderSource, GL_FRAGMENT_SHADER),
    m_id(GEM::Renderer::ShaderProgram::createShaderProgram(m_vertexShader.getID(), m_fragmentShader.getID()))
{}

/**
 * @brief Decrement the use count for this shader program. If the use count falls to 0 then we delete
 * the shader program
 */
GEM::Renderer::ShaderProgram::~ShaderProgram() {
    LOG_FUNCTION_CALL_TRACE("id {}", m_id);
    GEM::Renderer::ShaderProgram::decrementShaderProgramUseCount({m_vertexShader.getID(), m_fragmentShader.getID()});
}

/**
 * @brief Set this shader as the active shader using glUseProgram
 */
void GEM::Renderer::ShaderProgram::use() const {
    glUseProgram(m_id);
}

/**
 * @brief Set the value of a uniform representing a bool or vector of bools within the glsl shader
 * 
 * @param name The name of the uniform to set
 * @param value(s) The values to set the uniform to
 */

void GEM::Renderer::ShaderProgram::setUniformBool(const std::string& uniformName, const bool value) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform1i(uniformLocation, static_cast<int32_t>(value));
}
void GEM::Renderer::ShaderProgram::setUniformBVec2(const std::string& uniformName, const std::array<bool, 2>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform2i(uniformLocation, static_cast<int32_t>(values[0]), static_cast<int32_t>(values[1]));
}
void GEM::Renderer::ShaderProgram::setUniformBVec3(const std::string& uniformName, const std::array<bool, 3>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform3i(uniformLocation, static_cast<int32_t>(values[0]), static_cast<int32_t>(values[1]), static_cast<int32_t>(values[2]));
}
void GEM::Renderer::ShaderProgram::setUniformBVec4(const std::string& uniformName, const std::array<bool, 4>& values) {
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

void GEM::Renderer::ShaderProgram::setUniformInt(const std::string& uniformName, const int32_t value) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform1i(uniformLocation, value);
}
void GEM::Renderer::ShaderProgram::setUniformIVec2(const std::string& uniformName, const std::array<int32_t, 2>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform2i(uniformLocation, values[0], values[1]);
}
void GEM::Renderer::ShaderProgram::setUniformIVec3(const std::string& uniformName, const std::array<int32_t, 3>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform3i(uniformLocation, values[0], values[1], values[2]);
}
void GEM::Renderer::ShaderProgram::setUniformIVec4(const std::string& uniformName, const std::array<int32_t, 4>& values) {
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

void GEM::Renderer::ShaderProgram::setUniformUInt(const std::string& uniformName, const uint32_t value) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform1ui(uniformLocation, value);
}
void GEM::Renderer::ShaderProgram::setUniformUVec2(const std::string& uniformName, const std::array<uint32_t, 2>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform2ui(uniformLocation, values[0], values[1]);
}
void GEM::Renderer::ShaderProgram::setUniformUVec3(const std::string& uniformName, const std::array<uint32_t, 3>& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform3ui(uniformLocation, values[0], values[1], values[2]);
}
void GEM::Renderer::ShaderProgram::setUniformUVec4(const std::string& uniformName, const std::array<uint32_t, 4>& values) {
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

void GEM::Renderer::ShaderProgram::setUniformFloat(const std::string& uniformName, const float value) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform1f(uniformLocation, value);
}
void GEM::Renderer::ShaderProgram::setUniformVec2(const std::string& uniformName, const glm::vec2& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform2f(uniformLocation, values.x, values.y);
}
void GEM::Renderer::ShaderProgram::setUniformVec3(const std::string& uniformName, const glm::vec3& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform3f(uniformLocation, values.x, values.y, values.z);
}
void GEM::Renderer::ShaderProgram::setUniformVec4(const std::string& uniformName, const glm::vec4& values) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniform4f(uniformLocation, values.x, values.y, values.z, values.w);
}

/**
 * @brief Set the value of a matrix within the glsl shader
 * 
 * @param name The name of the uniform to set
 * @param matrix The matrix we are going to set it to
 */

void GEM::Renderer::ShaderProgram::setUniformMat2(const std::string& uniformName, const glm::mat2& matrix) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniformMatrix2fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

void GEM::Renderer::ShaderProgram::setUniformMat3(const std::string& uniformName, const glm::mat3& matrix) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

void GEM::Renderer::ShaderProgram::setUniformMat4(const std::string& uniformName, const glm::mat4& matrix) {
    const int uniformLocation = glGetUniformLocation(m_id, uniformName.c_str());
    if (uniformLocation == -1) {
        LOG_CRITICAL("Could not find location of uniform: {}", uniformName);
        return;
    }
    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

/**
 * @brief Set the value of a Sampler2D uniform representing a texture within the glsl shader
 * 
 * @param name The name of the uniform to set
 * @param texture The texture we are going to set it to (we use the texture's index)
 */

void GEM::Renderer::ShaderProgram::setUniformTextureSampler(const std::string& uniformName, std::shared_ptr<const GEM::Renderer::Texture> p_texture) {
    setUniformInt(uniformName, p_texture->getIndex());
}

/* ------------------------------ private member functions ------------------------------ */
