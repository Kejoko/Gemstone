#include <functional> // std::hash
#include <map>
#include <utility> // std::pair
#include <string>

#include <glad/glad.h>

#include "util/macros.hpp"
#include "util/logger/Logger.hpp"

#include "gemstone/renderer/shader/logger.hpp"
#include "gemstone/renderer/shader/CompiledShader.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the CompiledShader class uses
 */
const std::string GEM::Renderer::CompiledShader::LOGGER_NAME = SHADER_LOGGER_NAME;

/* ------------------------------ private static variables ------------------------------ */

/**
 * @brief Maps of the shader source code (hash) to the id of that source code.
 * This allows us to prevent compiling the same source code twice and instead just use pre
 * compiled shaders if attempting to use it subsequent times.
 * We are mapping the hash to a pair of (id, use count) so we can track how many times each
 * id is in use. This allows us to delete the shader if we decrement it to 0
 */
std::map<size_t, GEM::Renderer::CompiledShader::Info> GEM::Renderer::CompiledShader::vertexShaderIDMap;
std::map<size_t, GEM::Renderer::CompiledShader::Info> GEM::Renderer::CompiledShader::fragmentShaderIDMap;

/* ------------------------------ public static functions ------------------------------ */


/* ------------------------------ private static functions ------------------------------ */

/**
 * @brief Get a reference to the map we are using to track the desired type of shaders
 * 
 * @param shaderType The shader type
 * @return std::map<size_t, GEM::Renderer::CompiledShader::Info>& GEM::Renderer::CompiledShader::vertexShaderIDMap if
 * shaderType is GL_VERTEX_SHADER, GEM::Renderer::CompiledShader::fragmentShaderIDMap if shaderType is
 * GL_FRAGMENT_SHADER
 */
std::map<size_t, GEM::Renderer::CompiledShader::Info>& GEM::Renderer::CompiledShader::getShaderIDMap(const GLenum shaderType) {
    if (shaderType == GL_VERTEX_SHADER) {
        return GEM::Renderer::CompiledShader::vertexShaderIDMap;
    } else if (shaderType == GL_FRAGMENT_SHADER) {
        return GEM::Renderer::CompiledShader::fragmentShaderIDMap;
    }

    return GEM::Renderer::CompiledShader::fragmentShaderIDMap;
}

/**
 * @brief Convert the shader type enum to a string for printing purposes
 * 
 * @param shaderType The type of shader
 * @return std::string A string representation of the shader
 */
std::string GEM::Renderer::CompiledShader::getShaderTypeString(const GLenum shaderType) {
    if (shaderType == GL_VERTEX_SHADER) {
        return "Vertex";
    } else if (shaderType == GL_FRAGMENT_SHADER) {
        return "Fragment";
    }

    return "Unkown";
}

/**
 * @brief Add a shader to the map. Map it's hash to its id
 * 
 * @param shaderSourceHash The hash of the shader's source code
 * @param shaderID The id of the shader to map to its hash
 * @param shaderType The type of the shader
 */
void GEM::Renderer::CompiledShader::addShaderToMap(const size_t shaderSourceHash, const uint32_t shaderID, const GLenum shaderType) {
    LOG_FUNCTION_ENTRY_TRACE("hash {} , id {} , type {}", shaderSourceHash, shaderID, GEM::Renderer::CompiledShader::getShaderTypeString(shaderType));

    std::map<size_t, GEM::Renderer::CompiledShader::Info>& shaderIDMap = GEM::Renderer::CompiledShader::getShaderIDMap(shaderType);

    shaderIDMap.insert({shaderSourceHash, {shaderID, 0}});

    GEM::Renderer::CompiledShader::incrementShaderUseCount(shaderSourceHash, shaderType);
}

/**
 * @brief Increment the use count of the shader so we know how many programs are using it
 * 
 * @param shaderSourceHash The hash of the shader's source code
 * @param shaderType The type of shader
 */
void GEM::Renderer::CompiledShader::incrementShaderUseCount(const size_t shaderSourceHash, const GLenum shaderType) {
    LOG_FUNCTION_ENTRY_TRACE("hash {} , type {}", shaderSourceHash, GEM::Renderer::CompiledShader::getShaderTypeString(shaderType));

    std::map<size_t, GEM::Renderer::CompiledShader::Info>& shaderIDMap = GEM::Renderer::CompiledShader::getShaderIDMap(shaderType);

    GEM::Renderer::CompiledShader::Info info = shaderIDMap[shaderSourceHash];
    info.useCount = info.useCount + 1;
    
    LOG_TRACE("Use count for {} shader with id {} and hash {} is now: {}", GEM::Renderer::CompiledShader::getShaderTypeString(shaderType), info.id, shaderSourceHash, info.useCount);

    shaderIDMap[shaderSourceHash] = info;
}

/**
 * @brief Decrement the use count of the shader so we know how many programs are using it. If the count
 * reaches 0 then we are going to remove it from the map and delete the shader
 * 
 * @param shaderSourceHash The hash of the shader's source code
 * @param shaderType The type of shader
 */
void GEM::Renderer::CompiledShader::decrementShaderUseCount(const size_t shaderSourceHash, const GLenum shaderType) {
    LOG_FUNCTION_ENTRY_TRACE("hash {} , type {}", shaderSourceHash, GEM::Renderer::CompiledShader::getShaderTypeString(shaderType));

    std::map<size_t, GEM::Renderer::CompiledShader::Info>& shaderIDMap = GEM::Renderer::CompiledShader::getShaderIDMap(shaderType);
    
    GEM::Renderer::CompiledShader::Info info = shaderIDMap[shaderSourceHash];
    LOG_TRACE("Use count for {} shader with id {} and hash {} was: {}", GEM::Renderer::CompiledShader::getShaderTypeString(shaderType), info.id, shaderSourceHash, info.useCount);
    info.useCount = info.useCount - 1;

    LOG_TRACE("Use count for {} shader with id {} and hash {} is now: {}", GEM::Renderer::CompiledShader::getShaderTypeString(shaderType), info.id, shaderSourceHash, info.useCount);

    if (info.useCount > 0) {
        LOG_TRACE("Updating {} use count for shader with id {} and hash {}", GEM::Renderer::CompiledShader::getShaderTypeString(shaderType), info.id, shaderSourceHash);
        shaderIDMap[shaderSourceHash] = info;
        return;
    }

    LOG_TRACE("Erasing {} shader with id {} and hash {}", GEM::Renderer::CompiledShader::getShaderTypeString(shaderType), info.id, shaderSourceHash);
    shaderIDMap.erase(shaderSourceHash);

    LOG_TRACE("Deleting {} shader with id {}", GEM::Renderer::CompiledShader::getShaderTypeString(shaderType), info.id);
    glDeleteShader(info.id);
}

/**
 * @brief Determine if the shader we are given has already been compiled and is being tracked
 * 
 * @param shaderSourceHash The hash of the shader's source code
 * @param shaderType The type of the shader
 * @return true The hash has an entry in the map corresponding to the shaderType
 * @return false The hash does not have an entry in the map corresponding to the shaderType
 */
bool GEM::Renderer::CompiledShader::shaderIsCompiled(const size_t shaderSourceHash, const GLenum shaderType) {
    LOG_FUNCTION_ENTRY_TRACE("hash {} , type {}", shaderSourceHash, GEM::Renderer::CompiledShader::getShaderTypeString(shaderType));

    std::map<size_t, GEM::Renderer::CompiledShader::Info>& shaderIDMap = GEM::Renderer::CompiledShader::getShaderIDMap(shaderType);

    const uint32_t count = shaderIDMap.count(shaderSourceHash);
    bool isCompiled = count > 0;

    if (isCompiled) {
        LOG_TRACE("Found compiled {} shader for hash {}, count = {}", GEM::Renderer::CompiledShader::getShaderTypeString(shaderType), shaderSourceHash, count);
    }

    return isCompiled;
}

/**
 * @brief Take the source code of a shader and determine its hash value. This is so we can look
 * into the maps of shader ids
 * 
 * @param shaderSource The source code of the shader
 * @return size_t The hash of the shader's source code
 */
size_t GEM::Renderer::CompiledShader::getHashFromShaderSource(const char* shaderSource) {
    return std::hash<const char*>{}(shaderSource);
}

/**
 * @brief Get the id of the compiled shader
 * 
 * @param shaderSourceHash The hash of the shader's source code
 * @param shaderType The type of the shader
 * @return uint32_t The id of the shader
 */
uint32_t GEM::Renderer::CompiledShader::getCompiledShaderID(const size_t shaderSourceHash, const GLenum shaderType) {
    LOG_FUNCTION_ENTRY_TRACE("hash {} , type {}", shaderSourceHash, GEM::Renderer::CompiledShader::getShaderTypeString(shaderType));

    std::map<size_t, GEM::Renderer::CompiledShader::Info>& shaderIDMap = GEM::Renderer::CompiledShader::getShaderIDMap(shaderType);

    uint32_t shaderID = shaderIDMap[shaderSourceHash].id;
    LOG_TRACE("Got compiled {} shader id: {}", GEM::Renderer::CompiledShader::getShaderTypeString(shaderType), shaderID);

    return shaderID;
}

/**
 * @brief Compile a shader given it's source code and the type of shader 
 * 
 * @note This function will throw an exception if the shader fails to compile
 * 
 * @param shaderSource The source code of the shader
 * @param shaderType The type of the shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc ...)
 * @return uint32_t The compiled shader
 */
uint32_t GEM::Renderer::CompiledShader::compileShader(const char* shaderSource, const GLenum shaderType) {
    LOG_FUNCTION_CALL_INFO("{} shader", GEM::Renderer::CompiledShader::getShaderTypeString(shaderType));

    // Before we actually try to compile, check if this shader has already been compiled
    // If it has been compiled before, use that index and increment the counter
    const size_t shaderSourceHash = GEM::Renderer::CompiledShader::getHashFromShaderSource(shaderSource);
    if (GEM::Renderer::CompiledShader::shaderIsCompiled(shaderSourceHash, shaderType)) {
        uint32_t shaderID = GEM::Renderer::CompiledShader::getCompiledShaderID(shaderSourceHash, shaderType);

        GEM::Renderer::CompiledShader::incrementShaderUseCount(shaderSourceHash, shaderType);

        LOG_DEBUG("Successfully found compiled shader with id {}", shaderID);

        return shaderID;
    }

    /**
     * @todo Replace the string "#define MAX_NUMBER_DIRECTIONAL_LIGHTS -1" with the string containing the correct amoun
     * according to the preprocessor definition dictating the maximum number of that type of light for the scene. We must
     * do this for each of the three light types. For this to work we must set the three macros in the fragment shader
     * to be defined to -1 so the find and replace regex works nicely
     * 
     * @example The string ("#define MAX_NUMBER_POINT_LIGHTS -1") in the fragment shader gets changed to be the string
     * ("#define MAX_NUMBER_POINT_LIGHTS " + std::to_string(MAX_NUMBER_POINT_LIGHTS))
     */

    // Create a shader object, attach the shader source code, and compile
    // We must give the shader to compile, how many strings we are giving it as source,
    // the actual source code, and nullptr representing that the 1 source string is
    // null terminated
    uint32_t shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &shaderSource, nullptr);
    glCompileShader(shaderID);

    // Check for successful compilation of the shader
    int shaderCompilationSuccess;
    char shaderCompilationInfoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderCompilationSuccess);
    if (!shaderCompilationSuccess) {
        glGetShaderInfoLog(shaderID, sizeof(shaderCompilationInfoLog), nullptr, shaderCompilationInfoLog);

        const std::string errorMessage = getShaderTypeString(shaderType) + " shader failed to compile:\n" + std::string(shaderCompilationInfoLog);
        LOG_CRITICAL(errorMessage);
        throw std::invalid_argument(errorMessage);
    }

    GEM::Renderer::CompiledShader::addShaderToMap(shaderSourceHash, shaderID, shaderType);

    LOG_DEBUG("Successfully compiled shader program with id {}", shaderID);

    return shaderID;
}

/* ------------------------------ public member functions ------------------------------ */

GEM::Renderer::CompiledShader::CompiledShader(const char* shaderSource, const GLenum shaderType) :
    m_sourceHash(GEM::Renderer::CompiledShader::getHashFromShaderSource(shaderSource)),
    m_type(shaderType),
    m_id(GEM::Renderer::CompiledShader::compileShader(shaderSource, shaderType))
{}

GEM::Renderer::CompiledShader::~CompiledShader() {
    LOG_FUNCTION_CALL_TRACE("id {}", m_id);
    GEM::Renderer::CompiledShader::decrementShaderUseCount(m_sourceHash, m_type);
}

/* ------------------------------ private member functions ------------------------------ */