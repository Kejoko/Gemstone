#pragma once

#include <string>

#include <glad/glad.h>

#include "gemstone/shader/CompiledShader.hpp"

namespace GEM {
    class ShaderProgram;
}

/**
 * @brief A class wrapping the open gl shader program. This makes it easy for us to
 * create shader programs by simply supplying the vertex and fragment shader sources
 * then calling use()
 * 
 */
class GEM::ShaderProgram {
public:
    static const std::string LOGGER_NAME;

private:
    static uint32_t createShaderProgram(const uint32_t vertexShaderID, const uint32_t fragmentShaderID);
    
public:
    ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
    ~ShaderProgram();

    void use() const;

    uint32_t getID() const { return m_shaderID; }

private:
    const CompiledShader m_vertexShader;
    const CompiledShader m_fragmentShader;
    const uint32_t m_shaderID;
};