#pragma once

#include <array>
#include <string>
#include <type_traits>

#include <glad/glad.h>

#include "util/macros.hpp"

#include "gemstone/shader/CompiledShader.hpp"
#include "gemstone/texture/Texture.hpp"

namespace GEM {
    class ShaderProgram;
}

/**
 * @brief A class wrapping the open gl shader program. This makes it easy for us to
 * create shader programs by simply supplying the vertex and fragment shader sources
 * then calling use()
 */
class GEM::ShaderProgram {
public: // public static variables
    static const std::string LOGGER_NAME;
    
public: // public member functions
    ShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
    ~ShaderProgram();

    void use() const;

    uint32_t getID() const { return m_id; }

    /**
     * @todo Create a templated function for each of these to call.
     *        * we will need to implement a function to return the correct glUniform<n><t>v function
     *          as a pointer for us to call
     *           * <n> = 1, 2, 3, or 4 based on the number of args (length of uniform vec)
     *           * <t> = f, i, or ui based on the type of the parameters
     *           * we will need to figure out how to use these functions to pass arrays to them
     * 1) Use a std::array
     *        * we will need to use std::enable_if to only allow arrays of bool, int, uint, float
     *        * we will need to use std::enable_if to only arrays of lengths 1 - 4
     * 2) Use variadic arguments
     *        * we will need to ensure that all of the variables are of the same type
     *        * we will need to ensure that we only have 1 - 4 variadic arguments
     */

    void setUniformBool(const std::string& uniformName, const bool value);
    void setUniformBVec2(const std::string& uniformName, const std::array<bool, 2>& values);
    void setUniformBVec3(const std::string& uniformName, const std::array<bool, 3>& values);
    void setUniformBVec4(const std::string& uniformName, const std::array<bool, 4>& values);
    
    void setUniformInt(const std::string& uniformName, const int32_t value);
    void setUniformIVec2(const std::string& uniformName, const std::array<int32_t, 2>& values);
    void setUniformIVec3(const std::string& uniformName, const std::array<int32_t, 3>& values);
    void setUniformIVec4(const std::string& uniformName, const std::array<int32_t, 4>& values);

    void setUniformUInt(const std::string& uniformName, const uint32_t value);
    void setUniformUVec2(const std::string& uniformName, const std::array<uint32_t, 2>& values);
    void setUniformUVec3(const std::string& uniformName, const std::array<uint32_t, 3>& values);
    void setUniformUVec4(const std::string& uniformName, const std::array<uint32_t, 4>& values);
    
    void setUniformFloat(const std::string& uniformName, const float value);
    void setUniformVec2(const std::string& uniformName, const std::array<float, 2>& values);
    void setUniformVec3(const std::string& uniformName, const std::array<float, 3>& values);
    void setUniformVec4(const std::string& uniformName, const std::array<float, 4>& values);

    void setUniformTextureSampler(const std::string& uniformName, const GEM::Texture& texture);

private: // private static functions
    static uint32_t createShaderProgram(const uint32_t vertexShaderID, const uint32_t fragmentShaderID);

private: // private member variables
    const CompiledShader m_vertexShader;
    const CompiledShader m_fragmentShader;
    const uint32_t m_id;
};