#pragma once

#include <memory>
#include <string>

#include "gemstone/renderer/shader/ShaderProgram.hpp"
#include "gemstone/renderer/texture/Texture.hpp"

namespace GEM {
namespace Renderer{
    class Material;
}
}

class GEM::Renderer::Material {
public: // public static variables
    const static std::string LOGGER_NAME;

public: // public member functions
    Material(
        const std::string& diffuseMapFilename,
        const std::string& specularMapFilename,
        const float shininess,
        const char* vertexShaderSource,
        const char* fragmentShaderSource
    );
    ~Material();

    std::shared_ptr<const GEM::Renderer::Texture> getDiffuseMapPtr() const { return mp_diffuseMap; }
    std::shared_ptr<const GEM::Renderer::Texture> getSpecularMapPtr() const { return mp_specularMap; }
    float getShininess() const { return m_shininess; }

    std::shared_ptr<GEM::Renderer::ShaderProgram> getShaderProgramPtr() const { return mp_shaderProgram; }

    glm::vec3 getAmbientColor() const { return {1.0f, 0.5f, 0.31f}; }
    glm::vec3 getDiffuseColor() const { return {1.0f, 0.5f, 0.31f}; }
    glm::vec3 getSpecularColor() const { return {0.5f, 0.5f, 0.5f}; }

private: // private static variables
    static uint32_t materialCount;

private: // private static functions


private: // private member variables
    const uint32_t m_id;

    std::shared_ptr<GEM::Renderer::Texture> mp_diffuseMap;
    std::shared_ptr<GEM::Renderer::Texture> mp_specularMap;
    float m_shininess;

    std::shared_ptr<GEM::Renderer::ShaderProgram> mp_shaderProgram;

};