#pragma once

#include <string>

#include <glm/glm.hpp>

#include "gemstone/light/ObjectLight.hpp"

namespace GEM {
    class SpotLight;
};

class GEM::SpotLight : public GEM::ObjectLight {
public: // public static variables
    static const std::string LOGGER_NAME;

public: // public member functions
    SpotLight(
        const uint32_t id,
        const std::string& meshFilename,
        const std::string& diffuseMapFilename,
        const std::string& specularMapFilename,
        const std::string& emissionMapFilename,
        const float shininess,
        const char* vertexShaderSource,
        const char* fragmentShaderSource,
        const glm::vec3& initialWorldPosition,
        const glm::vec3& initialScale,
        const glm::vec3& initialRotationAxis,
        const float initialRotationAmountDegrees,
        const glm::vec3& initialDiffuseColor,
        const glm::vec3& initialSpecularColor,
        const float constant,
        const float linear,
        const float quadratic,
        const glm::vec3& initialDirection,
        const float radiusDegrees
    );
    ~SpotLight();

    glm::vec3 getDirection() const { return m_direction; }
    float getRadiusDegrees() const { return m_radiusDegrees; }

private: // private member functions

private: // private member variables
    glm::vec3 m_direction;
    float m_radiusDegrees;
};