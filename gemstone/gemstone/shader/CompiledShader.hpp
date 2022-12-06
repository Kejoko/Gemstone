#pragma once

#include <map>
#include <utility> // std::pair
#include <string>

#include <glad/glad.h>

namespace GEM {
    class CompiledShader;
}

/**
 * @brief A class that represents a compiled shader. When the use count hits 0, it should
 * call glDeleteShader and be removed from the shaderIDMap in the Shader class
 * 
 */
class GEM::CompiledShader {
public:
    static const std::string LOGGER_NAME;

private:
    struct Info {
        uint32_t id;
        int useCount;
    };

    static std::map<size_t, GEM::CompiledShader::Info> vertexShaderIDMap;
    static std::map<size_t, GEM::CompiledShader::Info> fragmentShaderIDMap;

    static std::map<size_t, GEM::CompiledShader::Info>& getShaderIDMap(const GLenum shaderType);

    static std::string getShaderTypeString(const GLenum shaderType);

    static void addShaderToMap(const size_t shaderSourceHash, const uint32_t shaderID, const GLenum shaderType);
    static void incrementShaderUseCount(const size_t shaderSourceHash, const GLenum shaderType);
    static void decrementShaderUseCount(const size_t shaderSourceHash, const GLenum shaderType);
    static bool shaderIsCompiled(const size_t shaderSourceHash, const GLenum shaderType);
    
    static size_t getHashFromShaderSource(const char* shaderSource);
    static uint32_t getCompiledShaderID(const size_t shaderSourceHash, const GLenum shaderType);
    static uint32_t compileShader(const char* shaderSource, const GLenum shaderType);

public:
    CompiledShader(const char* shaderSource, const GLenum shaderType);
    ~CompiledShader();
    
    size_t getSourceHash() const { return m_sourceHash; }
    GLenum getType() const { return m_type; }
    uint32_t getID() const { return m_id; }

private:
    const size_t m_sourceHash;
    const GLenum m_type;
    const uint32_t m_id;
};