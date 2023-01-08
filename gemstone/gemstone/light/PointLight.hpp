#pragma once

#include <string>

#include <glm/glm.hpp>

#include "gemstone/light/ObjectLight.hpp"

namespace GEM {
    class PointLight;
};

class GEM::PointLight : public GEM::ObjectLight {
public: // public static variables
    static const std::string LOGGER_NAME;

public: // public member functions
    PointLight(
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
        const float quadratic
    );
    ~PointLight();

private: // private member functions

private: // private member variables

};