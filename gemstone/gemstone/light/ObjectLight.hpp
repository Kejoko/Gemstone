#pragma once

#include <string>

#include <glm/glm.hpp>

#include "gemstone/light/Light.hpp"

namespace GEM {
    class ObjectLight;
};

class GEM::ObjectLight : public GEM::Object , public GEM::Light {
public: // public static variables
    static const std::string LOGGER_NAME;

public: // public member functions
    ObjectLight(
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
    virtual ~ObjectLight() = 0;

    float getConstant() const { return m_constant; }
    float getLinear() const { return m_linear; }
    float getQuadratic() const { return m_quadratic; }

private: // private member functions

protected: // private member variables
    float m_constant;
    float m_linear;
    float m_quadratic;
};