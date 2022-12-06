#include "gemstone/shader/CompiledShader.hpp"

#include <functional> // std::hash
#include <map>
#include <utility> // std::pair
#include <string>

#include <glad/glad.h>

#include "util/macros.hpp"
#include "util/logger/Logger.hpp"

/* ------------------------------ public static variables ------------------------------ */

/**
 * @brief The name of the logger the CompiledShader class uses
 */
const std::string GEM::CompiledShader::LOGGER_NAME = "SHADER";

/* ------------------------------ private static variables ------------------------------ */

/**
 * @brief Maps of the shader source code (hash) to the id of that source code.
 * This allows us to prevent compiling the same source code twice and instead just use pre
 * compiled shaders if attempting to use it subsequent times.
 * We are mapping the hash to a pair of (id, use count) so we can track how many times each
 * id is in use. This allows us to delete the shader if we decrement it to 0
 */
std::map<size_t, GEM::CompiledShader::Info> GEM::CompiledShader::vertexShaderIDMap;
std::map<size_t, GEM::CompiledShader::Info> GEM::CompiledShader::fragmentShaderIDMap;

/* ------------------------------ public static functions ------------------------------ */


/* ------------------------------ private static functions ------------------------------ */

std::map<size_t, GEM::CompiledShader::Info>& GEM::CompiledShader::getShaderIDMap(const GLenum shaderType) {
    if (shaderType == GL_VERTEX_SHADER) {
        return GEM::CompiledShader::vertexShaderIDMap;
    } else if (shaderType == GL_FRAGMENT_SHADER) {
        return GEM::CompiledShader::fragmentShaderIDMap;
    }

    return GEM::CompiledShader::fragmentShaderIDMap;
}

/**
 * @brief Convert the shader type enum to a string for printing purposes
 * 
 * @param shaderType The type of shader
 * @return std::string A string representation of the shader
 */
std::string GEM::CompiledShader::getShaderTypeString(const GLenum shaderType) {
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
void GEM::CompiledShader::addShaderToMap(const size_t shaderSourceHash, const uint32_t shaderID, const GLenum shaderType) {
    GEM::Logger::trace("Adding " + GEM::CompiledShader::getShaderTypeString(shaderType) + " shader id " + std::to_string(shaderID) + " to map for hash " + std::to_string(shaderSourceHash));

    std::map<size_t, GEM::CompiledShader::Info>& shaderIDMap = GEM::CompiledShader::getShaderIDMap(shaderType);

    shaderIDMap.insert({shaderSourceHash, {shaderID, 0}});

    GEM::CompiledShader::incrementShaderUseCount(shaderSourceHash, shaderType);
}

/**
 * @brief Increment the use count of the shader so we know how many programs are using it
 * 
 * @param shaderSourceHash The hash of the shader's source code
 * @param shaderType The type of shader
 */
void GEM::CompiledShader::incrementShaderUseCount(const size_t shaderSourceHash, const GLenum shaderType) {
    GEM::Logger::trace("Incrementing " + GEM::CompiledShader::getShaderTypeString(shaderType) + " shader use count for hash " + std::to_string(shaderSourceHash));

    std::map<size_t, GEM::CompiledShader::Info>& shaderIDMap = GEM::CompiledShader::getShaderIDMap(shaderType);

    GEM::CompiledShader::Info info = shaderIDMap[shaderSourceHash];
    info.useCount = info.useCount + 1;
    
    shaderIDMap[shaderSourceHash] = info;
    GEM::Logger::trace("Use count for " + GEM::CompiledShader::getShaderTypeString(shaderType) + " shader with hash " + std::to_string(shaderSourceHash) + " is now: " + std::to_string(info.useCount));
}

/**
 * @brief Decrement the use count of the shader so we know how many programs are using it. If the count
 * reaches 0 then we are going to remove it from the map and delete the shader
 * 
 * @param shaderSourceHash The hash of the shader's source code
 * @param shaderType The type of shader
 */
void GEM::CompiledShader::decrementShaderUseCount(const size_t shaderSourceHash, const GLenum shaderType) {
    GEM::Logger::trace("Decrementing " + GEM::CompiledShader::getShaderTypeString(shaderType) + " shader use count for hash " + std::to_string(shaderSourceHash));

    std::map<size_t, GEM::CompiledShader::Info>& shaderIDMap = GEM::CompiledShader::getShaderIDMap(shaderType);
    
    GEM::CompiledShader::Info info = shaderIDMap[shaderSourceHash];
    GEM::Logger::trace("Use count for " + GEM::CompiledShader::getShaderTypeString(shaderType) + " shader with hash " + std::to_string(shaderSourceHash) + " was: " + std::to_string(info.useCount));
    info.useCount = info.useCount - 1;

    GEM::Logger::trace("Use count for " + GEM::CompiledShader::getShaderTypeString(shaderType) + " shader with hash " + std::to_string(shaderSourceHash) + " is now: " + std::to_string(info.useCount));

    if (info.useCount > 0) {
        GEM::Logger::trace("Updating " + GEM::CompiledShader::getShaderTypeString(shaderType) + " shader with hash " + std::to_string(shaderSourceHash) + " use count in map");
        shaderIDMap[shaderSourceHash] = info;
        return;
    }

    GEM::Logger::trace("Erasing " + GEM::CompiledShader::getShaderTypeString(shaderType) + " shader with hash " + std::to_string(shaderSourceHash));
    shaderIDMap.erase(shaderSourceHash);

    GEM::Logger::trace("Deleting " + GEM::CompiledShader::getShaderTypeString(shaderType) + " shader with id " + std::to_string(info.id));
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
bool GEM::CompiledShader::shaderIsCompiled(const size_t shaderSourceHash, const GLenum shaderType) {
    GEM::Logger::trace("Checking if " + GEM::CompiledShader::getShaderTypeString(shaderType) + " shader is compiled for hash " + std::to_string(shaderSourceHash));

    std::map<size_t, GEM::CompiledShader::Info>& shaderIDMap = GEM::CompiledShader::getShaderIDMap(shaderType);

    const uint32_t count = shaderIDMap.count(shaderSourceHash);
    bool isCompiled = count > 0;

    if (isCompiled) {
        GEM::Logger::trace("Found compiled " + GEM::CompiledShader::getShaderTypeString(shaderType) + " shader for hash " + std::to_string(shaderSourceHash) + ", count = " + std::to_string(count));
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
size_t GEM::CompiledShader::getHashFromShaderSource(const char* shaderSource) {
    return std::hash<const char*>{}(shaderSource);
}

/**
 * @brief Get the id of the compiled shader
 * 
 * @param shaderSourceHash The hash of the shader's source code
 * @param shaderType The type of the shader
 * @return uint32_t The id of the shader
 */
uint32_t GEM::CompiledShader::getCompiledShaderID(const size_t shaderSourceHash, const GLenum shaderType) {
    GEM::Logger::trace("Getting compiled " + GEM::CompiledShader::getShaderTypeString(shaderType) + " shader id for hash " + std::to_string(shaderSourceHash));

    std::map<size_t, GEM::CompiledShader::Info>& shaderIDMap = GEM::CompiledShader::getShaderIDMap(shaderType);

    uint32_t shaderID = shaderIDMap[shaderSourceHash].id;
    GEM::Logger::trace("Got compiled " + GEM::CompiledShader::getShaderTypeString(shaderType) + " shader id: " + std::to_string(shaderID));

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
uint32_t GEM::CompiledShader::compileShader(const char* shaderSource, const GLenum shaderType) {
    LOG_FUNCTION_CALL(getShaderTypeString(shaderType) + " shader");

    // Before we actually try to compile, check if this shader has already been compiled
    // If it has been compiled before, use that index and increment the counter
    const size_t shaderSourceHash = GEM::CompiledShader::getHashFromShaderSource(shaderSource);
    if (GEM::CompiledShader::shaderIsCompiled(shaderSourceHash, shaderType)) {
        uint32_t shaderID = GEM::CompiledShader::getCompiledShaderID(shaderSourceHash, shaderType);

        GEM::CompiledShader::incrementShaderUseCount(shaderSourceHash, shaderType);

        return shaderID;
    }

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
        GEM::Logger::critical(errorMessage);
        throw std::invalid_argument(errorMessage);
    }

    GEM::CompiledShader::addShaderToMap(shaderSourceHash, shaderID, shaderType);

    return shaderID;
}

/* ------------------------------ public member functions ------------------------------ */

GEM::CompiledShader::CompiledShader(const char* shaderSource, const GLenum shaderType) :
    m_sourceHash(GEM::CompiledShader::getHashFromShaderSource(shaderSource)),
    m_type(shaderType),
    m_id(GEM::CompiledShader::compileShader(shaderSource, shaderType))
{}

GEM::CompiledShader::~CompiledShader() {
    GEM::Logger::info("Deconstructing compiled shader with id " + std::to_string(m_id));
    GEM::CompiledShader::decrementShaderUseCount(m_sourceHash, m_type);
}

/* ------------------------------ private member functions ------------------------------ */