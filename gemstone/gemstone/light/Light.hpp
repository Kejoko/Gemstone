#pragma once

#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "gemstone/light/logger.hpp"
#include "gemstone/object/Object.hpp"
#include "gemstone/renderer/shader/ShaderProgram.hpp"

namespace GEM {
    class Light;
}

class GEM::Light : public GEM::Object {
public: // public static variables
    static const std::string LOGGER_NAME;

public: // public member functions
    Light(
        const uint32_t id,
        const std::string& meshFilename,
        const std::string& textureFilename,
        const std::string& textureFilename2,
        const float shininess,
        const char* vertexShaderSource,
        const char* fragmentShaderSource,
        const glm::vec3& initialWorldPosition,
        const glm::vec3& initialScale,
        const glm::vec3& initialRotationAxis,
        const float initialRotationAmountDegrees,
        const glm::vec3& initialDiffuseColor,
        const glm::vec3& initialSpecularColor
    );
    ~Light();

    glm::vec3 getDiffuseColor() const { return m_diffuseColor; }
    glm::vec3 getSpecularColor() const { return m_specularColor; }

private: // private member variables
    glm::vec3 m_diffuseColor;
    glm::vec3 m_specularColor;
};